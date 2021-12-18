(ns aoc-day14
  (:require [clojure.string :as string]))


;;; Input data ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn parse
  [data-string]
  (let [lines (string/split data-string #"\n")]
    {:pat (first lines)
     :rls (map #(string/split % #" -> ")
               (drop 2 lines))}))

(def sample (parse (slurp "data/day14.sample")))
(def full (parse (slurp "data/day14.full")))


;;; Helpers ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn zero-map
  [data f]
  (reduce #(assoc %1 (f %2) 0) {} data))

(defn pairs->map
  [pairs]
  (reduce #(assoc %1 (first %2) (second %2))
                   {}
                   pairs))

(defn inc-map
  [m k]
  (assoc m k (inc (get m k))))

(defn init-pair-counts
  [data]
  (reduce
    #(assoc %1
            (first %2) (count (re-seq
                                (re-pattern (first %2))
                                (:pat data))))
    {}
    (:rls data)))

(defn init-letter-counts
  [data]
  (merge (zero-map (:rls data) second)
         (frequencies (map str (:pat data)))))

(defn process-pair
  [pair n rules-map]
  (let [rep (get rules-map pair)
        front (str (first pair) rep)
        back (str rep (second pair))]
    {:pairs {front n back n}
     :letters {rep n}}))

(defn merge-add
  [m1 m2]
  (reduce #(assoc %1
                  (first %2) (+ (second %2)
                                (get %1 (first %2))))
          m1
          m2))

(defn process-rules
  [rules pairs letters]
  (loop [rs rules
         ps (zero-map rules first)
         ls letters]
    (if (empty? rs)
      {:pairs ps :letters ls}
      (let [p (first (first rs))
            x (process-pair p (get pairs p) (pairs->map rules))]
        (if (= 0 (get pairs p))
          (recur (rest rs) ps ls)
          (recur (rest rs)
                 (merge-add ps (:pairs x))
                 (merge-add ls (:letters x))))))))

(defn process-n
  [rules pairs letters n]
  (loop [ps pairs
         ls letters
         i 0]
    (if (= i n)
      ls
      (let [x (process-rules rules ps ls)]
        (recur (:pairs x) (:letters x) (inc i))))))


;;; Solutions ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn solve
  [data n]
  (let [fq (process-n (:rls data)
                      (init-pair-counts data)
                      (init-letter-counts data)
                      n)
        counts (sort (map val fq))]
    (- (last counts) (first counts))))

(solve sample 10) ; 1588
(solve full 10) ; 3213

(solve sample 40) ; 2188189693529
(solve full 40) ; 3711743744429

