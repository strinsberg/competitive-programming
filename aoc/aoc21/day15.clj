(ns aoc-day14
  (:require [clojure.string :as string]))

;;; Input data ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn parse
  [data-string]
  (->> (string/split data-string #"\n")
       (map
         (fn [line]
           (map #(Character/digit % 10) line)))))

(def sample (parse (slurp "data/day15.sample")))
(def full (parse (slurp "data/day15.full")))

;;; Helpers ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn inc-map
  [m k v]
  (if (contains? m k)
    (assoc m k (+ v (get m k)))
    (assoc m k v)))

(defn inc-item
  [x n]
  (let [i (+ x n)]
    (if (< i 10)
      i
      (inc (mod i 10)))))

(defn inc-row
  [row n]
  (map #(inc-item % n) row))

(defn add-data-map
  [M v]
  (map #(inc-row % v) M))

(defn grow-row
  [row n]
  (loop [final ()
         i 0]
    (if (= i n)
      final
      (recur (concat final (inc-row row i))
             (inc i)))))

(defn grow-map
  [n M]
  (->> (range n)
       (mapcat #(add-data-map M %))
       (map #(grow-row % n))))

;;; Grid edges ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn get-value
  [M [r c]]
  (nth (nth M r) c))

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
  [[r c] m n costs deltas]
  (reduce
    (fn [acc [dr dc]]
      (if-let [p (adj-point r c dr dc m n)]
        (conj acc {:v [r c] :u p :c (get-value costs p)})
        acc))
    []
    deltas))

(defn edge-list
  "Creates an weighted edge list from a matrix, where each cell has the cost of
  moving into that space. Uses given deltas for valid directional movements."
  [deltas data]
  (flatten
    (let [m (count data)
          n (count (first data))]
      (for [r (range m)
            c (range n)]
        (get-adjacent [r c] m n data deltas)))))

;;; Graph ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;; NOTE this way of building a graph is really slow. It would be better to
;; use a different data structure. THERE IS A BETTER WAY, DO NOT USE THIS!!!

(defn graph [] {})

(defn g-add-edge
  [G {:keys [v u c]}]
  (assoc G v (conj (get G v #{}) {:u u :c c})))

(defn edges->graph
  [edges]
  (reduce #(g-add-edge %1 %2) (graph) edges))

;;; Priority Queue ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn priority-q
  []
  (sorted-map))

(defn pq-insert
  [Q k v]
  (if (get Q k)
    (assoc Q k (conj (get Q k) v))
    (assoc Q k #{v})))

(defn pq-pop
  [Q]
  (let [head (first Q)
        k (first head)
        items (second head)]
    (if (= 1 (count items))
      (dissoc Q k)
      (assoc Q k (disj items (first items))))))

(defn pq-peek
  [Q]
  (let [head (first Q)
        k (first head)
        items (second head)]
    [k (first items)]))

;;; Dikstra ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(def INF 1000000000)

(defn dijkstra
  [start init G]
  (loop [Q (-> (priority-q) (pq-insert init start))
         dist (-> {} (inc-map start init))
         seen #{}]
    (if (empty? Q)
      dist
      (let [[gone v] (pq-peek Q)
            to (filter #(and (not (contains? seen (:u %)))
                             (< (+ gone (:c %))
                                (get dist (:u %) INF)))
                       (get G v))
            new-dist (reduce #(inc-map %1 (:u %2) (+ gone (:c %2)))
                             dist
                             to)
            new-Q (reduce #(pq-insert %1 (get new-dist (:u %2)) (:u %2))
                          (pq-pop Q)
                          to)]
        (if (contains? seen v)
          (recur (pq-pop Q) dist seen)
          (recur new-Q new-dist (conj seen v)))))))

;;; Solutions ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn solve-a
  [data end]
  (get
    (->> data
         (edge-list deltas)
         edges->graph
         (dijkstra [0 0] 0))
    end))

(solve-a sample [9 9])
(time (solve-a full [99 99]))

(defn solve-b
  [data end]
  (get
    (->> data
         (grow-map 5)
         (edge-list deltas)
         edges->graph
         (dijkstra [0 0] 0))
    end))

(solve-b sample [49 49])
(time (solve-b full [499 499]))

;; 60+ sec
(def full-graph
  (time
    (->> full
         (grow-map 5)
         (edge-list deltas)
         edges->graph
    )))

;; 10 sec
(time (get (dijkstra [0 0] 0 full-graph) [499 499]))
