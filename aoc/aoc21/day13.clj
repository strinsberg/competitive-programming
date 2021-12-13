(ns aoc-day13
  (:require [clojure.string :as string]))


;;; Input data ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn parse
  [data-string]
  (string/split data-string #"\n"))

(def sample (parse (slurp "data/day13.sample")))
(def full (parse (slurp "data/day13.full")))


;;; Helpers ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defn get-dots
  [data]
  (->> data
       (filter #(contains? (set %) \,))
       (map #(string/split % #","))
       (map (fn [xs]
              (map #(Integer/parseInt %) xs)))
       set))

(defn get-size
  [f dots]
  (inc (apply max (flatten (map f dots)))))

(defn get-folds
  [data]
  (->> data
       (filter #(contains? (set %) \=))
       (map #(string/split % #"="))
       (map #(list (last (first %))
                   (Integer/parseInt (second %))))))

(defn make-page
  [x y dots]
  (for [r (range y)]
    (for [c (range x)]
      (if (contains? dots (list c r)) 1 0))))

;; Folding is kind of messy because I used map for it with 2 lists.
;; Because if the two maps are not the same size the result has the smaller
;; size then it is possible to lose rows or columns if the fold is not in
;; the center. To solve this and not have to rethink my folds I just set
;; up a split function for each that pads the shorter list with 0
;; rows or columns so that the fold is done as expected. This is not really
;; too complicated, but it is a lot going on. It also does a lot of counting
;; and concatting which is not very efficient.
;; It would be good to rethink the problem and then see if there is a better
;; way of doing the folds more concisely.

(defn fold-y
  [i page]
  (let [[t b] (split-y i page)]
    (map (fn [r1 r2]
           (map #(bit-or %1 %2) r1 r2))
         t
         b)))

(defn pad-rows
  [n lines]
  (concat
    (for [i (range n)]
      (for [j (range (count (first lines)))]
        0))
    lines))

(defn split-y
  [i page]
  (let [top (take i page)
        bottom (reverse (drop (inc i) page))
        nt (count top)
        nb (count bottom)]
    (cond
      (> nt nb) [top (pad-rows (- nt nb) bottom)]
      (> nb nt) [(pad-rows (- nb nt) top) bottom]
      :else [top bottom])))

(defn fold-x
  [i page]
  (map (fn [line]
         (let [[r l] (split-x i line)]
           (map #(bit-or %1 %2) r l)))
       page))

(defn pad
  [n row]
  (concat (for [i (range n)] 0) row))

(pad 3 '(1 1 1))

(defn split-x
  [i page]
  (let [left (take i page)
        right (reverse (drop (inc i) page))
        nl (count left)
        nr (count right)]
    (cond
      (> nl nr) [left (pad (- nl nr) right)]
      (> nr nl) [(pad (- nr nl) left) right]
      :else [left right])))

(defn fold-all
  [folds page]
  (loop [fs folds
         pg page]
    (prn (first fs) (count pg) (count (first pg)))
    (cond
      (empty? fs) pg
      (= \y (first (first fs)))
         (recur (rest fs) (fold-y (second (first fs)) pg))
      :else
         (recur (rest fs) (fold-x (second (first fs)) pg)))))

;;; Solutions ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn solve-a
  [data]
  (->>
    (let [dots (get-dots data)
          fold (first (get-folds data))
          x (get-size first dots)
          y (get-size second dots)
          page (make-page x y dots)]
      (if (= \y (first fold))
        (fold-y (second fold) page)
        (fold-x (second fold) page)))
    flatten
    (apply +)))

(solve-a sample) ; 17
(solve-a full) ; 701

(defn solve-b
  [data]
  (let [dots (get-dots data)
        folds (get-folds data)
        x (get-size first dots)
        y (get-size second dots)
        page (make-page x y dots)]
    (fold-all folds page)))
       
(solve-b sample)
; ((1 1 1 1 1)
;  (1 0 0 0 1)
;  (1 0 0 0 1)
;  (1 0 0 0 1)
;  (1 1 1 1 1)
;  (0 0 0 0 0)
;  (0 0 0 0 0))

(map prn (solve-b full))
; (1 1 1 1  1 1 1 0  1 1 1 1  1 0 0 1  1 1 1 0  1 1 1 1  0 0 1 1  1 0 0 0 )
; (1 0 0 0  1 0 0 1  1 0 0 0  1 0 1 0  1 0 0 1  1 0 0 0  0 0 0 1  1 0 0 0 )
; (1 1 1 0  1 0 0 1  1 1 1 0  1 1 0 0  1 1 1 0  1 1 1 0  0 0 0 1  1 0 0 0 )
; (1 0 0 0  1 1 1 0  1 0 0 0  1 0 1 0  1 0 0 1  1 0 0 0  0 0 0 1  1 0 0 0 )
; (1 0 0 0  1 0 0 0  1 0 0 0  1 0 1 0  1 0 0 1  1 0 0 0  1 0 0 1  1 0 0 0 )
; (1 0 0 0  1 0 0 0  1 1 1 1  1 0 0 1  1 1 1 0  1 1 1 1  0 1 1 0  1 1 1 1 )


