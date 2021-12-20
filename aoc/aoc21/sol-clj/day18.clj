(ns aoc-day18
  (:require [clojure.string :as string]))

;; Managed to get things to work without extra tree traversals to check for
;; for things like explode? or split?. Instead we just run one of the correct
;; functions and it will walk the tree, the return telling whether or not it
;; happened. Now instead of doing a check and then an operation it is just the
;; operation, and only one operation is done at a time. If an operation does
;; not happen it will move on to the next. For example if a split happens on
;; the left no processing will happen on the right, and in add if an explode
;; was needed it will happen and recurse without checking for splits.
;;
;; To get this to work and be somewhat clean I split some things into a lot
;; of small functions. It may not be strictly necessary, but it made each
;; seem more manageable. However, the check left and then do right if needed
;; has created a little more nesting with let and if. It is not hard to read
;; once you look at it, but a little harder to see what is happening at a glance.

;;; Input data ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(def sample (read-string (str "[" (slurp "data/day18.sample") "]")))
(def sample2 (read-string (str "[" (slurp "data/day18.sample2") "]")))
(def full (read-string (str "[" (slurp "data/day18.full") "]")))

;;; Explode ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(declare snail-exp)

(defn leaf?
  [tree]
  (and (int? (first tree))
       (int? (second tree))))

(defn leaf-explode
  [tree depth]
  (if (>= depth 4)
    {:t 0 :l (first tree) :r (second tree) :ex true}
    {:t tree}))

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

(defn left-explode
  [left right]
  (if (:r left)
      (assoc left :r nil
                  :t [(:t left)
                      (add-leftmost (:r left) right)])
      (assoc left :t [(:t left) right])))

(defn right-explode
  [right left]
  (if (:l right)
    (assoc right :l nil
                 :t [(add-rightmost (:l right) left)
                     (:t right)])
    (assoc right :t [left (:t right)])))

(defn tree-explode
  [tree depth]
  (let [left (snail-explode (first tree) (inc depth))]
    (if (:ex left)
      (left-explode left (second tree))
      (let [right (snail-explode (second tree) (inc depth))]
        (if (:ex right)
          (right-explode right (first tree))
          {:t tree})))))

(defn snail-explode
  [tree depth]
  (cond
    (int? tree) {:t tree}
    (leaf? tree) (leaf-explode tree depth)
    :else (tree-explode tree depth)))

;;; Split ;;;

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

;;; Add and Magnitude ;;;

(defn snail-add
  [a b]
  (loop [tree [a b]]
    (let [exp (snail-explode tree 0)]
      (if (:ex exp)
        (recur (:t exp))
        (let [sp (snail-split tree)]
          (if (:sp sp)
            (recur (:t sp))
            tree))))))

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

(solve-b sample) ; 3805
(solve-b sample2) ; 3993
(solve-b full) ; 4650

