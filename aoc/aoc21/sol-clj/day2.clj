(ns aoc-day2
  (:require [clojure.string :as string]))

;;; Input data ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn parse
  [data-string]
  (map #(let [[dir amt] (string/split %1 #" +")]
          [dir (read-string amt)]) 
        (string/split data-string #"\n")))

(def sample (parse (slurp "data/day2.sample")))
(def full (parse (slurp "data/day2.full")))

;;; Heplers ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn step-a
  [[x y aim] [dir amt]]
  (cond
    (= dir "forward") [(+ x amt) y aim]
    (= dir "up") [x (- y amt) aim]
    :else [x (+ y amt) aim]))

(defn step-b
  [[x y aim] [dir amt]]
  (cond
    (= dir "forward") [(+ x amt) (+ y (* aim amt)) aim]
    (= dir "up") [x y (- aim amt) aim]
    :else [x y (+ aim amt)]))

(defn move
  [step-fn commands]
  (reduce
    (fn [acc cmd] (step-fn acc cmd))
    [0 0 0]
    commands))

;;; Solutions ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn solve-a
  [data]
    (let [[x y _] (move step-a data)]
      (* x y)))

(solve-a sample) ; 150
(solve-a full) ; 1924923

(defn solve-b
  [data]
  (let [[x y _] (move step-b data)]
    (* x y)))
       
(solve-b sample) ; 900
(solve-b full) ; 1982495697

