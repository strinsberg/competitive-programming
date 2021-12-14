(ns aoc-day14
  (:require [clojure.string :as string]))


;;; Input data ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn parse
  [data-string]
  (string/split data-string #"\n"))

(def sample (parse (slurp "data/day14.sample")))
(def full (parse (slurp "data/day14.full")))


;;; Helpers ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defn get-pattern
  [data]
  (first data))

(defn get-rules
  [data]
  (map #(string/split % #" -> ")
       (drop 2 data)))

(get-pattern sample)
(get-rules sample)

(defn splits
  [p pattern]
  (let [rep (str (first p) \space (second p))
        p1 (string/replace pattern p rep)
        p2 (string/replace p1 p rep)]
      (string/split p2 #" ")))

(str (first "NN") \space (second "NN"))
(splits "NN" "NNCB")
(splits "BB" "NBBBBC")

(def step
  (memoize (fn [pattern rules]
    (if (empty? rules)
      pattern
      (let [[pat rep] (first rules)
            sp (splits pat pattern)]
        (string/join rep
                     (map #(step % (rest rules))
                          sp)))))))
(step (get-pattern sample) (get-rules sample))

(defn n-step
  [data n]
  (loop [p (get-pattern data)
         r (get-rules data)
         i 0]
    (if (= i n)
      p
      (recur (step p r) r (inc i)))))
(count (n-step sample 2))
(count (n-step sample 3))
(count (n-step sample 10))

;;; Solutions ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn solve-a
  [data]
  (let [fq (frequencies (n-step data 10))
        counts (sort (map val fq))]
    (- (last counts) (first counts))))

(solve-a sample) ; 1588
(solve-a full) ; 3213

(defn solve-b
  [data]
  (let [fq (frequencies (n-step data 40))
        counts (sort (map val fq))]
    (- (last counts) (first counts))))
       
(solve-b sample)
(solve-b full)

;;; Other ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; fix this so that all that is here is the correct way to do it
;; simplify the process so that it is all done in a chain of functions
;; rather than the mess of creating all of these things and leaving them
;; Remove some of the inefficiencies


(defn zero-map
  [data f]
  (reduce #(assoc %1 (f %2) 0) {} data))

(defn pairs->map
  [pairs]
  (reduce #(assoc %1 (first %2) (second %2))
                   {}
                   pairs))

(def pattern (get-pattern sample))
(def rules (get-rules sample))
(def rules-map (pairs->map rules))
(def rule-acc (zero-map rules first))
(def acc (zero-map rules second))
(def pairs (merge  rule-acc
                  (reduce
                    #(assoc %1
                            (first %2)
                            (count (re-seq (re-pattern (first %2))
                                           pattern)))
                    {}
                    rules)))
(def letters (merge acc (frequencies (map str pattern))))

;; for each of the pairs a step will increase the number of thier value,
;; the pair (str (first pair) val), and (str val (second pair))
(defn inc-map
  [m k]
  ;(prn k m)
  (assoc m k (inc (get m k))))
(inc-map letters "H")


(defn process-pair
  [pair n rules-map]
  (let [rep (get rules-map pair)
        front (str (first pair) rep)
        back (str rep (second pair))]
    {:pairs {front n back n}
     :letters {rep n}}))

(process-pair "NB" 1 rules-map)
(process-pair "BC" 2 rules-map)

(defn merge_
  [m1 m2]
  (reduce #(assoc %1
                  (first %2) (+ (second %2)
                                (get %1 (first %2))))
          m1
          m2))

(merge_ {:a 1 :b 3} {:a 0 :b 2})

(defn process-rules
  [rules pairs letters]
  ;(prn "")
  ;(prn "  " pairs)
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
                 (merge_ ps (:pairs x))
                 (merge_ ls (:letters x))))))))

(process-rules rules pairs letters)

(defn process-n
  [rules pairs letters n]
  (loop [ps pairs
         ls letters
         i 0]
    (if (= i n)
      ls
      (let [x (process-rules rules ps ls)]
        (recur (:pairs x) (:letters x) (inc i))))))

(process-n rules pairs letters 1)
(process-n rules pairs letters 2)
(process-n rules pairs letters 3)
(process-n rules pairs letters 5)
(process-n rules pairs letters 10)
(def counts-s (process-n rules pairs letters 40))
(- (apply max (flatten (map second counts-s)))
   (apply min (flatten (map second counts-s))))


(def pattern-f (get-pattern full))
(def rules-f (get-rules full))
(def rules-map-f (pairs->map rules-f))
(def rule-acc-f (zero-map rules-f first))
(def acc-f (zero-map rules-f second))
(def pairs-f (merge  rule-acc-f
                  (reduce
                    #(assoc %1
                            (first %2)
                            (count (re-seq (re-pattern (first %2))
                                           pattern-f)))
                    {}
                    rules-f)))
(def letters-f (merge acc-f (frequencies (map str pattern-f))))

(process-n rules-f pairs-f letters-f 10)
(def counts-f (process-n rules-f pairs-f letters-f 40))
(- (apply max (flatten (map second counts-f)))
   (apply min (flatten (map second counts-f)))) ; 3711743744429
