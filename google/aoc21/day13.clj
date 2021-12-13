(ns aoc-day13
  (:require [clojure.string :as string]))


;;; Input data ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn parse
  [data-string]
  (->> (string/split data-string #"\n")
       identity))

(def sample (parse (slurp "data/day13.sample")))
(def full (parse (slurp "data/day13.full")))


;;; Helpers ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;


;;; Solutions ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn solve-a
  [data]
  )

(solve-a sample)
(solve-a full)

(defn solve-b
  [data]
  )
       
(solve-b sample)
(solve-b full)

