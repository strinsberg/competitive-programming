(ns aoc-day12
  (:require [clojure.string :as string]))


;;; Input data ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn parse
  "Split the input into pairs of strings representing edges in the graph.
  Note these are undirected edges."
  [data-string]
  (->> (string/split data-string #"\n")
       (map #(string/split % #"-"))))

(def sample (parse (slurp "data/day12.sample")))
(def full (parse (slurp "data/day12.full")))


;;; Helpers ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defn string-pairs->adj-map
  "Given a list of string pairs representing an edge from v->u in an undirected
  graph, returns an adjacency map of nodes to sets of their connected nodes.
  Converts strings to keywords for each node name."
  [data]
  (->> (set data)
       (reduce (fn [acc x] (-> acc (conj x) (conj (reverse x)))) #{})
       (map #(map keyword %))
       (group-by first)
       (map (fn [[k v]] [k (set (filter #(not= k %) (flatten v)))]))
       (reduce (fn [acc [k v]] (assoc acc k v)) {})))

(defn get-small-caves
  "Specifically for this problem find all node names with fully lowercase
  string names and create a set of them as keywords. This set is the set of
  small caves."
  [data]
  (->> (set (flatten data))
       (filter (fn [node-set] (every? #(Character/isLowerCase %) node-set)))
       (filter #(not (contains? #{"end" "start"} %)))
       (map keyword)
       set))

(defn cave-graph
  "Takes the list of string edge pairs and creates an adjacency map from them
  converted to keywords. Maps :small to the set of small caves."
  [data]
  (assoc (string-pairs->adj-map data)
         :small (get-small-caves data)))

;; Part A we want to walk the graph and pass on a seen set that we put only
;; the lower case (and start) into. At each node we recurse on all unseen
;; children and return the sum of the results. If we find end, we return 1.
(def paths
  (memoize
    (fn [v seen G]
      (if (= v :end)
        1
        (let [next_ (filter #(not (contains? seen %)) (v G))
              seen_ (if (contains? (:small G) v) (conj seen v) seen)]
          (reduce + 0 (map #(paths % seen_ G) next_)))))))

;; Same as A but when looking for which children can be looked at
;; we can go down one small path twice. Not start though. Probably could
;; use a map with counts instead of a set for seen, and seed it with
;; :small from G with 0 for all entries, but 2 for :start (and :end)
(def paths-2
  (memoize
    (fn [v seen G sm?]
      (if (= v :end)
        1
        (let [next_  (filter #(and (not= :start %)
                                   (or (not (contains? seen %))
                                       (not sm?)))
                             (v G))
              seen_ (if (contains? (:small G) v) (conj seen v) seen)]
          (apply + (map #(paths-2 % seen_ G (or sm? (contains? seen %)))
                        next_)))))))

;;; Solutions ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn solve-a
  [data]
  (paths :start
         #{:start}
         (cave-graph data)))

(solve-a sample) ; 19
(solve-a full) ; 3761

(defn solve-b
  [data]
  (paths-2 :start
           #{:start}
           (cave-graph data)
           false))
       
(solve-b sample) ; 103
(solve-b full) ; 99138

