(ns aoc-day17
  (:require [clojure.string :as string]))

;;; Input data ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(def sample {:x1 20 :x2 30 :y1 -5 :y2 -10})
(def full {:x1 29 :x2 73 :y1 -194 :y2 -248})

;;; Helpers ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn update-vx
  [v]
  (cond
    (> v 0) (dec v)
    (< v 0) (inc v)
    :else 0))

(defn step
  [{:keys [x y vx vy]}]
  {:x (+ x vx)
   :y (+ y vy)
   :vx (update-vx vx)
   :vy (dec vy)})

(defn out-of-range
  [{:keys [x y vx vy]} {:keys [x1 y1 x2 y2]}]
  (or (> x x2)
      (< y y2)))

(defn in-range
  [{:keys [x y vx vy]} {:keys [x1 y1 x2 y2]}]
  (and (>= x x1) (<= x x2) (<= y y1) (>= y y2)))

(defn simulate-path
  [[vx vy] dest]
  (loop [probe {:x 0 :y 0 :vx vx :vy vy}
         max-y (:y probe)
         i 0]
    (cond
      (or (out-of-range probe dest) (> i 10000)) {:y nil :probe probe}
      (in-range probe dest) {:y max-y :probe probe}
      :else (let [p-next (step probe)]
              (recur p-next (max max-y (:y p-next)) (inc i))))))

;;; Solutions ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn solve-a
  [data]
  (->> (for [x (range (inc (:x2 data)))
             y (range 1000)]
         (simulate-path [x y] data))
       flatten
       (map :y)
       (filter identity)
       (apply max)))

(solve-a sample) ; 45
(solve-a full) ; 30628


(defn solve-b
  [data]
  (->> (for [x (range (inc (:x2 data)))
             y (range (:y2 data) 1000)]
         (simulate-path [x y] data))
       flatten
       (map :y)
       (filter identity)))

(count (solve-b sample)) ; 112
(count (solve-b full)) ; 4433


