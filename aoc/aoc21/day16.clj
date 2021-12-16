(ns aoc-day16
  (:require [clojure.string :as string]))

;;; Input data ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(def full (slurp "data/day16.full"))

;;; Helpers ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn hex-digit-to-bits
  [digit]
  (get
    {\0 "0000"
     \1 "0001"
     \2 "0010"
     \3 "0011"
     \4 "0100"
     \5 "0101"
     \6 "0110"
     \7 "0111"
     \8 "1000"
     \9 "1001"
     \A "1010"
     \B "1011"
     \C "1100"
     \D "1101"
     \E "1110"
     \F "1111"}
    digit))

(defn hex-to-binary
  [hex-string]
  (map #(Character/digit % 10)
       (mapcat hex-digit-to-bits hex-string)))

(defn decode-type
  [bits]
  (get
    {'(0 0 0) +
     '(0 0 1) *
     '(0 1 0) (fn [a b] (min a b))
     '(0 1 1) (fn [a b] (max a b))
     '(1 0 0) :lit
     '(1 0 1) (fn [a b] (if (> a b) 1 0))
     '(1 1 0) (fn [a b] (if (< a b) 1 0))
     '(1 1 1) (fn [a b] (if (= a b) 1 0))}
    (take 3 (drop 3 bits))
    :op))

(defn to-int
  [bits]
  (read-string (str "2r" (apply str bits))))

;;; Parsing ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(declare parse-packet)

(defn parse-lit
  [bits]
  (loop [bxs bits
         result '()]
    (if (or (= 0 (first bxs))
            (empty? bxs))
      {:val (to-int (concat result
                            (take 4 (rest bxs))))
       :bits (drop 5 bxs)}
      (recur (drop 5 bxs)
             (concat result (take 4 (rest bxs)))))))

(defn parse-bit-packets
  [len-bits op bits]
  (loop [res (parse-packet (take len-bits bits))
         acc (:val res)]
    (if (< (count (:bits res)) 10)
      {:val acc
       :bits (drop len-bits bits)}
      (let [x (parse-packet (:bits res))]
        (recur x (op acc (:val x)))))))

(defn parse-n-packets
  [n op bits]
  (reduce (fn [acc _]
            (let [res (parse-packet (:bits acc))]
              {:val (op (:val acc) (:val res))
               :bits (:bits res)}))
          (parse-packet bits)
          (range (dec n))))

(defn parse-op
  [op bits]
  (let [len (if (= 0 (first bits)) 15 11)
        len-bits (to-int (take len (rest bits)))
        bxs (drop len (rest bits))]
    (if (= 15 len)
      (parse-bit-packets len-bits op bxs)
      (parse-n-packets len-bits op bxs))))

(defn parse-packet
  [bits]
  (let [op (decode-type bits)
        bxs (drop 6 bits)]
    (if (= :lit op)
      (parse-lit bxs)
      (parse-op op bxs))))

;;; Solutions ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;; Part A is removed because it was lame ;)

(defn solve-b
  [data]
  (->> data
       hex-to-binary
       parse-packet
       :val))

(solve-b full) ; 10626195124371

