(ns aoc-day3
  (:require [clojure.string :as string]))

;;; Input data ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn parse
  [data-string]
  (mapv (fn [line] (mapv #(Character/digit % 10) line))
        (string/split data-string #"\n")))

(def sample (parse (slurp "data/day3.sample")))
(def full (parse (slurp "data/day3.full")))

;;; Heplers ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn common-digit
  [data col least?]
  (let [sum (apply + (map #(nth % col 0) data))]
    (if (< (* 2 sum) (count data))
      (bit-xor 0 least?)
      (bit-xor 1 least?))))

(defn bin-digits-to-dec
  [digits]
  (->> digits
       (string/join "")
       (str "2r")
       read-string))

(defn calc-power
  [data epsilon?]
  (->> (map #(common-digit data % epsilon?)
           (range (count (first data))))
       bin-digits-to-dec))

(defn calc-life
  [data co2?]
  (loop [res data
         j 0]
    (if (= 1 (count res))
      (bin-digits-to-dec (first res))
      (let [cd (common-digit res j co2?)]
        (recur (vec (filter #(= (nth % j 0) cd) res)) (inc j))))))

;;; Solutions ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn solve-a
  [data]
  (* (calc-power data 0)
     (calc-power data 1)))

(solve-a sample) ; 198
(solve-a full) ; 738234

(defn solve-b
  [data]
  (* (calc-life data 0)
     (calc-life data 1)))
       
(solve-b sample) ; 230
(solve-b full) ; 3969126


