(ns aoc-day12
  (:require [clojure.string :as string]))


;;; Input data ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Builds an adjacency list, but there has got to be a better way ;)
(defn parse
  [data-string]
  (->> (string/split data-string #"\n")
       (map #(string/split % #"-"))
       (reduce
         (fn [acc [a b]]
           (let [v (keyword a)
                 u (keyword b)
                 add-v-u (if (v acc)
                           (assoc acc v (conj (v acc) u))
                           (assoc acc v #{u}))
                 add-u-v (if (u add-v-u)
                           (assoc add-v-u u (conj (u add-v-u) v))
                           (assoc add-v-u u #{v}))
                 add-a-s (if (every? #(Character/isLowerCase %) a)
                           (assoc add-u-v :small (conj (:small add-u-v) v))
                           add-u-v)]
             (if (every? #(Character/isLowerCase %) b)
                           (assoc add-a-s :small (conj (:small add-a-s) u))
                           add-a-s)))
         {:small #{}})))

(def sample (parse (slurp "data/day12.sample")))
(def full (parse (slurp "data/day12.full")))


;;; Helpers ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Part A we want to walk the graph and pass on a seen set that we put only
;; the lower case (and start) into. At each node we recurse on all unseen
;; children and return the sum of the results. If we find end, we return 1.
(defn paths
  [v seen G]
  (if (= v :end)
    1
    (let [next_ (filter #(not (contains? seen %))
                        (get G v))
          new-seen (if (contains? (:small G) v)
                     (conj seen v)
                     seen)]
      (reduce + 0 (map #(paths % new-seen G) next_)))))

;; Same as A but when looking for which children can be looked at
;; we can go down one small path twice. Not start though. Probably could
;; use a map with counts instead of a set for seen, and seed it with
;; :small from G with 0 for all entries, but 2 for :start (and :end)
(defn paths-2
  [v seen G sm2?]
  (if (= v :end)
    1
    (let [next_ (filter #(not= :start %)
                        (filter #(or (not (contains? seen %))
                                     (not sm2?))
                                (v G)))
          new-seen (if (contains? (:small G) v)
                     (conj seen v)
                     seen)]
      (reduce + 0 (map #(paths-2 % new-seen G (or sm2? (contains? seen %))) next_)))))

;;; Solutions ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn solve-a
  [data]
  (paths :start #{:start} data))

(solve-a sample) ; 19
(solve-a full) ; 3761

(defn solve-b
  [data]
  (paths-2 :start #{:start} data false))
       
(solve-b sample) ; 103
(solve-b full) ; 99138

