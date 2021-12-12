(ns aoc-day9
  (:require [clojure.string :as string]))

;;; Input ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn parse
  [data-string]
  (->> (string/split data-string #"\n")
       (map string/trim)
       (map vec)
       (mapv (fn [row] (mapv #(Character/digit % 10) row)))))

(def sample-string
  "2199943210
   3987894921
   9856789892
   8767896789
   9899965678")

(def sample (parse sample-string))
(def full (parse (slurp "data/day9.full")))

;;; Part A ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(def deltas [[0 1] [1 0] [0 -1] [-1 0]])

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

(defn get-low-coords
  "Get all coordinates in the data table that have a lower value than all
  adjacent coordinates. Always lower than adjacent cells that are
  not in bounds off the table."
  [data m n]
  (for [r (range m)
        c (range n)
        :when (every?
                #(< (get-value data [r c])
                    (get-value data %))
                (get-adjacent [r c] m n deltas))]
    [r c]))

(defn get-basin
  "Returns the set of all coordinates that are in the basin with the low-point.
  Assumes that there is only one low point per basin, essentially meaning that
  the only way a point is not in the basin is that its value is 9."
  [data low-point m n]
  (loop [points [low-point]
         seen #{low-point}
         basin #{}]
    (if (empty? points)
      basin
      (let [p (peek points)
            next_ (filter #(and (not= 9 (get-value data %))
                                (not (contains? seen %)))
                          (get-adjacent p m n))]
        (recur (vec (concat (pop points) next_))
               (clojure.set/union seen (set next_))
               (conj basin p))))))

;; Solutions A
(defn solve-a
  [data]
  (let [m (count data)
        n (count (first data))]
    (->> (get-low-coords data m n)
         (map #(inc (get-value data %)))
         (apply +))))

(solve-a sample) ; 15
(time (solve-a full)) ; 478

;; Solutions B
(defn solve-b
  [data]
  (let [m (count data)
        n (count (first data))]
    (->> (get-low-coords data m n)
         (map #(get-basin data % m n))
         (map count)
         (sort >)
         (take 3)
         (apply *))))

(solve-b sample) ; 1134
(time (solve-b full)) ; 1327014


;;; Testing ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(in-bounds? 0 0 5 5) ; true
(in-bounds? 4 4 5 5) ; true
(in-bounds? 5 5 5 5) ; false
(in-bounds? 10 0 5 5) ; false
(in-bounds? 0 10 5 5) ; false
(in-bounds? -1 4 5 5) ; false
(in-bounds? 4 -1 5 5) ; false

(adj-point 0 0 0 1 5 5) ; [0 1]
(adj-point 0 0 1 0 5 5) ; [1 0]
(adj-point 0 0 0 -1 5 5) ; nil
(adj-point 0 0 -1 0 5 5) ; nil

(get-adjacent [0 0] 5 5) ; [[0 1] [1 0]]
(get-adjacent [2 2] 5 5) ; [[2 3] [3 2] [2 1] [1 2]]

(get-value sample [0 0]) ; 2
(get-value sample [2 2]) ; 5
(get-value sample [0 9]) ; 0

(get-low-coords sample) ; ([0 1] [0 9] [2 2] [4 6])
(time (get-low-coords full))

(def sample-m (count sample))
(def sample-n (count (first sample)))

(get-basin sample [0 1] sample-m sample-n)
(get-basin sample [0 9] sample-m sample-n)
(get-basin sample [2 2] sample-m sample-n)
(get-basin sample [4 6] sample-m sample-n)

