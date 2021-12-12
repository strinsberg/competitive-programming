(ns aoc-day10
  (:require [clojure.string :as string]))


;;; Input data ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn parse
  [data-string]
  (string/split data-string #"\n"))

(def sample (parse (slurp "data/day10.sample")))
(def full (parse (slurp "data/day10.full")))


;;; Helpers ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(def pairs {\( \) \[ \] \{ \} \< \>})
(def scores {\) 3 \] 57 \} 1197 \> 25137})
(def scores-b {\) 1 \] 2 \} 3 \> 4})

(defn closed-by? [a b] (= b (get pairs a)))

(defn nav-parse
  "Parses a line by matching the opening and closing pairs.
  If an unexpected closer is found then the line is corrupted and we return
  the offending char. Otherwise the stack is returned. If it is empty then all
  openers had a closer at the proper place otherwise starting at the top
  each corresponding closer is required to complete it."
  [line]
  (loop [nav (vec line)
         stack []]
    (cond
      (empty? nav)
        stack
      (get pairs (first nav))
        (recur (rest nav) (conj stack (first nav)))
      (closed-by? (peek stack) (first nav))
        (recur (rest nav) (pop stack))
      :else
        (first nav))))

(defn get-completion
  "Returns a seq of closers that would need to be added to the given
  stack to make it complete. The closer that matches the top of the stack
  is first in the seq"
  [stack]
  (reverse (map #(get pairs %) stack)))

(defn get-completion-scores
  "Returns a seq with the corresponding completion scores for each
  character in the completion."
  [completion]
  (map #(get scores-b %) completion))

(defn calculate-total
  "Calculates the total for a list of completion scores as directed."
  [scores]
  (reduce (fn [acc x] (+ x (* acc 5))) 0 scores))

(defn take-middle
  "Takes the middle element of an odd length seq."
  [xs]
  (first (drop (quot (count xs) 2) xs)))


;;; Solutions ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn solve-a
  [data]
  (->> data
       (map nav-parse)
       (filter #(char? %))
       (map #(get scores %))
       (apply +)))

(solve-a sample) ; 26397
(solve-a full) ; 469755

(defn solve-b
  [data]
  (->> data
       (map nav-parse)
       (filter #(not (char? %)))
       (map get-completion)
       (map get-completion-scores)
       (map calculate-total)
       sort
       take-middle))
       
(solve-b sample) ; 288957
(solve-b full) ; 2762335572

