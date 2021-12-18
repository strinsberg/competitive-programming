(ns aoc-day1
  (:require [clojure.string :as string]))

;;; Input data ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn parse
  [data-string]
  (mapv read-string (string/split data-string #"\n")))

(def sample (parse (slurp "data/day1.sample")))
(def full (parse (slurp "data/day1.full")))

;;; Heplers ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn num-inc-from-prev
  [data]
  (->> data
       (reduce
         (fn [[acc last_] x]
           [(if (> x last_) (inc acc) acc) x])
         [0, 10000000000])
       first))

(defn window-sums
  "Returns a vec of sums for each full window size in a given seq of nums"
  [nums size]
  (loop [low 0
         high size
         sums [(reduce + (take size nums))]]
    (if (nth nums high nil)
      (recur (inc low)
             (inc high)
             (conj sums (-> (peek sums)
                            (- (nth nums low))
                            (+ (nth nums high)))))
      sums)))

;;; Solutions ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn solve-a
  [data]
  (num-inc-from-prev data))

(solve-a sample) ; 7
(solve-a full) ; 1184

(defn solve-b
  [data]
  (-> data
      (window-sums 3)
      num-inc-from-prev))
       
(solve-b sample) ; 5
(solve-b full) ; 1158

