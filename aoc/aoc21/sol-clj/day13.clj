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


;;; Process the folds ;;;

(defn split-group
  [i v]
  (cond
    (<= v i) :keep
    (= v i) :lose
    :else :fold))

(defn split
  [i dots y?]
  (let [f (if y? second first)]
    (group-by #(split-group i (f %)) dots)))

(defn reflect-y
  [i [x y]]
  (list x (- i (- y i))))

(defn reflect-x
  [i [x y]]
  (list (- i (- x i)) y))

(defn fold
  [[axis i] dots]
  (let [grouped (split i dots (= axis \y))
        reflect (if (= axis \y) reflect-y reflect-x)]
    (reduce
      #(conj %1 (reflect i %2))
      (set (:keep grouped))
      (set (:fold grouped)))))

(defn process-folds
  [folds dots]
  (loop [fs folds
         ds dots]
    (if (empty? fs)
      ds
      (recur (rest fs) (fold (first fs) ds)))))

(defn make-page
  [x y dots]
  (for [r (range y)]
    (apply
      str
      (for [c (range x)]
        (if (contains? dots (list c r)) "#" " ")))))


;;; Solutions ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn solve-a
  [data]
  (let [dots (get-dots data)
        folds (get-folds data)]
    (count (fold (first folds) dots))))

(solve-a sample) ; 17
(solve-a full) ; 701

;; Note the use of make-page will create a smaller result page than
;; expected if rows or columns on the outer edges have no dots. This is
;; OK for the intended solution, but may not work in other situations.
;; For that the initial size will need to be processed with the folds to
;; see what the page size will be at the end.
(defn solve-b
  [data]
  (let [dots (process-folds (get-folds data) (get-dots data))
        x (get-size first dots)
        y (get-size second dots)]
    (make-page x y dots)))
       
(solve-b sample)
; ("#####"
;  "#   #"
;  "#   #"
;  "#   #"
;  "#####")

(solve-b full)
; ("#### ###  #### #  # ###  ####   ## #   "
;  "#    #  # #    # #  #  # #       # #   "
;  "###  #  # ###  ##   ###  ###     # #   "
;  "#    ###  #    # #  #  # #       # #   "
;  "#    #    #    # #  #  # #    #  # #   "
;  "#    #    #### #  # ###  ####  ##  ####")


