(ns aoc-day18
  (:require [clojure.string :as string]))

;;; Input data ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(def sample (read-string (str "[" (slurp "data/day18.sample") "]")))
(def sample2 (read-string (str "[" (slurp "data/day18.sample2") "]")))
(def full (read-string (str "[" (slurp "data/day18.full") "]")))

;;; New ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defn add-leftmost
  [v node]
  (if (int? node)
    (+ v node)
    [(add-leftmost v (first node)) (second node)]))

(defn add-rightmost
  [v node]
  (if (int? node)
    (+ v node)
    [(first node) (add-rightmost v (second node))]))

(defn explode?
  [t d]
  (cond
    (int? t) false
    (>= d 4) true
    :else (or (explode? (first t) (inc d))
              (explode? (second t) (inc d)))))

(defn snail-explode
  [[left right] depth]
  (cond
    (explode? left (inc depth))
    (let [l (snail-explode left (inc depth))
          add-r (if (:r l) (add-leftmost (:r l) right) right)]
        {:t [(:t l) add-r] :l (:l l)})

    (explode? right (inc depth))
    (let [r (snail-explode right (inc depth))
          add-l (if (:l r) (add-rightmost (:l r) left) left)]
        {:t [add-l (:t r)] :r (:r r)})

    (>= depth 4) {:l left :r right :t 0}
    :else {:t [left right]}))

(defn split?
  [tree]
  (not-every? #(<= % 9) (flatten tree)))

(defn div-up
  [n]
  (let [i (/ n 2)]
    (if (int? i)
      i
      (int (inc i)))))

(defn snail-split
  [tree]
  (if (int? tree)
    (if (> tree 9)
      {:t [(int (/ tree 2)) (div-up tree)] :sp true}
      {:t tree})
    (let [l (snail-split (first tree))]
      (if (:sp l)
        (assoc l :t [(:t l) (second tree)])
        (let [r (snail-split (second tree))]
          (assoc r :t [(first tree) (:t r)]))))))

(defn snail-add
  [a b]
  (loop [tree [a b]
         i 0]
    (cond
      (explode? tree 0) (recur (:t (snail-explode tree 0)) (inc i))
      (split? tree) (recur (:t (snail-split tree)) (inc i))
      :else tree)))

(defn magnitude
  [tree]
  (if (int? tree)
    tree
    (+ (* 3 (magnitude (first tree)))
       (* 2 (magnitude (second tree))))))

;;; Solutions ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn solve-a
  [data]
  (magnitude
    (reduce
      snail-add
      data)))


(solve-a sample) ; 3488
(solve-a sample2) ; 4140
(solve-a full) ; 3675


(defn solve-b
  [data]
  (->> (for [a data
             b data
             :when (not= a b)]
         (magnitude (snail-add a b)))
       flatten
       (apply max)))

(solve-b sample)
(solve-b sample2) ; 3993
(solve-b full) ; 4650

