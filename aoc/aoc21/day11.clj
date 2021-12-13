(ns aoc-day11
  (:require [clojure.string :as string]))


;;; Input data ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn parse
  [data-string]
  (->> (string/split data-string #"\n")
       (map string/trim)
       (map vec)
       (mapv (fn [row] (mapv #(Character/digit % 10) row)))))

(def sample (parse (slurp "data/day11.sample")))
(def full (parse (slurp "data/day11.full")))


;;; Helpers ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(def deltas [[0 1] [1 0] [0 -1] [-1 0] [-1 -1] [1 1] [-1 1] [1 -1]])

(defn in-bounds?
  [r c m n]
  (and (< r m) (< c n) (nat-int? r) (nat-int? c)))

(defn adj-point
  "Given the coordinates of a cell in a table, and deltas, returns the
  coord resulting from adding the deltas if will be inside the table.
  Otherwise returns nil."
  [r c dr dc m n]
  (let [nr (+ r dr)
        nc (+ c dc)]
    (if (in-bounds? nr nc m n)
      [nr nc]
      nil)))

(defn get-adjacent
  "Given the coodinates of a cell in a table returns the adjacent cells
  provided they are also in the table."
  [[r c] m n deltas]
  (reduce
    (fn [acc [dr dc]]
      (if-let [p (adj-point r c dr dc m n)]
        (conj acc p)
        acc))
    []
    deltas))

(defn get-value
  "Get the value of a cell in a table at the given coordinate. Assumes that
  the coordinate is in the table."
  [data [r c]]
  (nth (nth data r) c))


;;; Solution specific ;;;

(defn increase-energy
  "Increase the energy of each squid by 1"
  [data]
  (mapv
    (fn [row]
      (mapv #(inc %) row))
    data))

(defn get-flashed
  "Return the coords for each cell in a 2d vector that has a value of 10.
  This is only used after the increase so there is no chance that a squid will
  have greater than 10. And if it was used during flash chaining it would
  not take things greater than 10 because they have already flashed once."
  [data m n]
  (vec (for [r (range m)
        c (range n)
        :when (= (get-value data [r c]) 10)]
    [r c])))

(defn increase
  "Increase a given cell in a 2d data vector"
  [data [r c]]
  (let [row (nth data r)
        x (inc (nth row c))]
    (assoc data r (assoc row c x))))

(defn reset-flashed
  "All squid that were flashed during a step go back to 0"
  [row]
  (mapv #(if (> % 9) 0 %) row))

(defn flash-all
  "Given a set of squid that have flashed during a step performs flash chaining
  on each one of them. No squid will flash twice, but any squid next to a squid
  when it flashes will have its energy increased. If this increase is enough
  to flash it then it will be added to the list and flashed."
  [data m n flashed]
  (loop [points flashed
         new-data data
         i 0]
    (if (empty? points)
      {:data (mapv #(reset-flashed %) new-data) :flashes i}
      (let [p (peek points)
            adj (get-adjacent p m n deltas)
            ;; Increase the energyof the squid and allthe adjacent ones
            updated (reduce #(increase %1 %2) new-data (conj adj p))
            ;; Only add squid that are at 10 and will flash, anything
            ;; over 10 has already flashed
            next_ (filter #(= 10 (get-value updated %)) adj)]
        (recur (vec (concat (pop points) next_))
               updated
               (inc i))))))

(defn step
  "Increases all the energy levels and flashes all those over 9. Each flash
  chains with flash-all to neighboring ones if it brings thier energy high
  enough to flash. No squid will flash twice."
  [data m n]
  (let [updated (increase-energy data)
        flashed (get-flashed updated m n)]
    (flash-all updated m n flashed)))

(defn all-flashed?
  "Just checks to see that the data is all zeros"
  [data]
  (= 0 (apply + (mapcat identity data))))

;;; Solutions ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn solve-a
  [data]
  (let [m (count data)
        n (count (first data))
        steps 100]
    (loop [squid data
           i 0
           flashes 0]
      (if (< i 100)
        (let [new-data (step squid m n)]
        (recur (:data new-data)
               (inc i)
               (+ flashes (:flashes new-data))))
        {:data squid :flashes flashes}))))

(solve-a sample)
; {:data
;  [[0 3 9 7 6 6 6 8 6 6]
;   [0 7 4 9 7 6 6 9 1 8]
;   [0 0 5 3 9 7 6 9 3 3]
;   [0 0 0 4 2 9 7 8 2 2]
;   [0 0 0 4 2 2 9 8 9 2]
;   [0 0 5 3 2 2 2 8 7 7]
;   [0 5 3 2 2 2 2 9 6 6]
;   [9 3 2 2 2 2 8 9 6 6]
;   [7 9 2 2 2 8 6 8 6 6]
;   [6 7 8 9 9 9 8 7 6 6]],
;  :flashes 1656}

(solve-a full)
; {:data
;  [[5 5 6 8 4 5 5 9 7 6]
;   [5 6 8 6 4 0 0 0 9 7]
;   [6 8 5 8 0 0 0 0 0 9]
;   [8 7 2 4 0 0 0 0 0 0]
;   [1 4 2 3 5 0 0 0 0 0]
;   [1 5 2 2 3 4 5 9 8 0]
;   [2 5 2 2 2 2 2 7 5 5]
;   [0 6 3 2 2 2 2 7 4 4]
;   [0 0 5 3 2 2 2 6 4 4]
;   [0 0 0 3 2 2 7 5 4 4]],
;  :flashes 1743}

(defn solve-b
  [data]
  (let [m (count data)
        n (count (first data))
        steps 100]
    (loop [squid data
           i 0]
      (if (all-flashed? squid)
        i
        (let [new-data (step squid m n)]
          (recur (:data new-data) (inc i)))))))
       
(solve-b sample) ; 195
(solve-b full) ; 364


;;; Testing ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(increase-energy sample)
; [[6 5 9 4 2 5 4 3 3 4]
;  [3 8 5 6 9 6 5 8 2 2]
;  [6 3 7 5 6 6 7 2 8 4]
;  [7 2 5 2 4 4 7 2 5 7]
;  [7 4 6 8 4 9 6 5 8 9]
;  [5 2 7 8 6 3 5 7 5 6]
;  [3 2 8 7 9 5 2 8 3 2]
;  [7 9 9 3 9 9 2 2 4 5]
;  [5 9 5 7 9 5 9 6 6 5]
;  [6 3 9 4 8 6 2 6 3 7]]

(def m (count sample))
(def n (count (first sample)))
(def after-step-2 (increase-energy (increase-energy sample)))
(def flashed (get-flashed after-step-2 m n))
; ([0 2]
;  [1 4]
;  [4 5]
;  [4 9]
;  [6 4]
;  [7 1]
;  [7 2]
;  [7 4]
;  [7 5]
;  [8 1]
;  [8 4]
;  [8 6]
;  [9 2])

(increase [[1 1] [1 1]] [1 1]) ; [[1 1] [1 2]]

(flash-all after-step-2 m n (vec (reverse flashed)))
; {:data
;  [[8 8 0 7 4 7 6 5 5 5]
;   [5 0 8 9 0 8 7 0 5 4]
;   [8 5 9 7 8 8 9 6 0 8]
;   [8 4 8 5 7 6 9 6 0 0]
;   [8 7 0 0 9 0 8 8 0 0]
;   [6 6 0 0 0 8 8 9 8 9]
;   [6 8 0 0 0 0 5 9 4 3]
;   [0 0 0 0 0 0 7 4 5 6]
;   [9 0 0 0 0 0 0 8 7 6]
;   [8 7 0 0 0 0 6 8 4 8]],
;  :flashes 35}

(step after-step-2 m n)
; {:data
;  [[9 9 0 8 6 9 9 7 6 6]
;   [6 0 0 0 0 0 0 0 7 5]
;   [9 9 0 0 0 0 0 0 0 9]
;   [9 7 0 0 0 0 0 0 0 0]
;   [9 9 0 0 0 0 0 0 0 0]
;   [7 7 0 0 0 9 0 0 0 0]
;   [5 6 0 0 0 9 7 0 9 6]
;   [9 0 0 8 0 0 5 5 7 7]
;   [7 0 8 0 0 8 0 8 8 7]
;   [8 5 0 8 0 9 4 8 5 9]],
;  :flashes 39}

(all-flashed? [[0 0 0] [0 0 0]]) ; true
(all-flashed? [[0 1 0] [0 0 0]]) ; false

