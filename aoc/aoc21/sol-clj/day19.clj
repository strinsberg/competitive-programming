(ns aoc-day19
  (:require [clojure.string :as string]))

;; This was made to work in the repl way down at the bottom. Never really got
;; it to work the way the rest do. The major issue is not really knowing how
;; to do it properly, so the algorithm is very naive and brute force, just
;; testing almost all of the combinations, and reproducing all the rotations
;; for each time that we do things. Some memoization on some things could help
;; or just rewriting it to create all the rotations once and then using them
;; after than. There is a better way... this takes a long time to run. But for
;; a first solution it works and I now know all that needs to be done, plus
;; where some of the serious bottlenecks are in the brute force version.
;; Later I will look at Howards solution to see what the better way is, as well
;; as perhaps think about how to improve my clojure code to make this version
;; better as well.

;;; Data ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn parse
  [text-data]
  (->> (clojure.string/split text-data #"\n\n")
       (map (fn [scanner]
              (rest (clojure.string/split scanner #"\n"))))
       (map (fn [scanner]
              (set (map #(read-string (str "[" % "]"))
                   scanner))))))

(def sample (parse (slurp "data/day19.sample")))
(def full (parse (slurp "data/day19.full")))
(map count sample)
(map count full)
(count full)

;;; Helpers ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

(defn shift-point
  [[dx dy dz] [x y z]]
  [(+ dx x) (+ dy y) (+ dz z)])

(defn find-delta
  [[x1 y1 z1] [x2 y2 z2]]
  [(- x1 x2) (- y1 y2) (- z1 z2)])

(defn shift-to
  [delta points]
  (set (map #(shift-point delta %) points)))


;; Rotations are in no particular clockwise or counter clockwise orientation
;; they can simply generate all the 4 sets of points if they were rotated
;; If there had to be a guaranteed order to the rotations this would not work
;; as expected for each, but all we need is the rotations so we can get all
;; 24 possible orientations

(defn rotate-x
  [points times]
  (case times
    1 (map (fn [point] [(first point) (nth point 2) (- (second point))]) points)
    2 (map (fn [point] [(first point) (- (second point)) (- (nth point 2))]) points)
    3 (map (fn [point] [(first point) (- (nth point 2)) (second point)]) points)
    0 points
    nil points))

(defn rotate-y
  [points times]
  (case times
    1 (map (fn [point] [(nth point 2) (second point) (- (first point))]) points)
    2 (map (fn [point] [(- (first point)) (second point) (- (nth point 2))]) points)
    3 (map (fn [point] [(- (nth point 2)) (second point) (first point)]) points)
    0 points
    nil points))

(defn rotate-z
  [points times]
  (case times
    1 (map (fn [point] [(second point) (- (first point)) (nth point 2)]) points)
    2 (map (fn [point] [(- (first point)) (- (second point)) (nth point 2)]) points)
    3 (map (fn [point] [(- (second point)) (first point) (nth point 2)]) points)
    0 points
    nil points))

(defn all-rotations
  [points]
  (let [x-pos (for [r (range 4)] {:x r :points (set (rotate-x points r))})
        x-neg (for [r (range 4)] {:x r :y 2
                                  :points (set (rotate-x (rotate-y points 2) r))})
        y-pos (for [r (range 4)] {:y r :z 1
                                  :points (set (rotate-y (rotate-z points 1) r))})
        y-neg (for [r (range 4)] {:y r :z 3
                                  :points (set (rotate-y (rotate-z points 3) r))})
        z-pos (for [r (range 4)] {:z r :y 1
                                  :points (set (rotate-z (rotate-y points 1) r))})
        z-neg (for [r (range 4)] {:z r :y 3
                                  :points (set (rotate-z (rotate-y points 3) r))})]
    (set (concat x-pos x-neg y-pos y-neg z-pos z-neg))))

(def points [[-1,-1,1]
             [-2,-2,2]
             [-3,-3,3]
             [-2,-3,1]
             [5,6,-4]
             [8,0,7]])
(rotate-y #{[1 2 3]} 3)

[1  2  3] ;; facing x-pos
[1  3 -2]
[1 -2 -3]
[1 -3  2]

[-1  2 -3] ;; facing x-neg
[-1 -3 -2]
[-1 -2  3]
[-1  3  2]

[ 2 -1  3] ;; facing y-pos
[ 3 -1 -2]
[-2 -1 -3]
[-3 -1  2]

[-2 1  3] ;; facing y-neg
[ 3 1  2]
[ 2 1 -3]
[-3 1 -2]

[ 3  2 -1] ;; facing z-pos
[ 2 -3 -1]
[-3 -2 -1]
[-2  3 -1]

[-3  2 1] ;; facing z-neg
[-2  3 1]
[ 3 -2 1]
[ 2 -3 1]

(def rot (all-rotations points))
(count rot) 
(filter #(contains? (:points %) [1 -1 -1]) rot)

(defn rotate-each
  [{:keys [x y z]} point]
  (-> #{point}
      (rotate-x x)
      (rotate-y y)
      (rotate-z z)))

;;;;;;;

(def set1 #{[0 0 0] [8 8 8] [15 15 15] [4 4 4]})
(def set2 #{[1 1 1] [10 10 10] [16 16 16] [5 5 5]})

(defn get-shared
  [points other min-amt]
  (first
    (for [a points
          b other
          :let [delta (find-delta a b)
                shifted (shift-to delta other)
                common (clojure.set/intersection points shifted)]
          :when (>= (count common) min-amt)]
      {:delta delta :common common :normalized shifted})))

(get-shared set1 set2 3)

(defn find-overlap
  [points other min-amt]
  (loop [rotations (all-rotations other)
         acc nil]
    (if (empty? rotations)
      acc
      (recur (rest rotations)
             (if-let [common (get-shared points (:points (first rotations)) min-amt)]
               (merge common (dissoc (first rotations) :points))
               acc)))))

(defn all-overlaps
  [data min-amt]
  (for [i (range (count data))
        j (range (count data))
        :let [overlap (find-overlap (nth data i) (nth data j) min-amt)]
        :when (and (< i j) (:delta overlap))]
    (assoc overlap :i i :j j)))

(def overlaps (all-overlaps sample 12))
(def filt-over (filter #(and (not= 0 (:j %)) (not= [0 0 0] (:delta %))) overlaps))
(count overlaps)
(group-by :i filt-over)

(def ov1 (find-overlap (nth sample 0) (nth sample 1) 12))
(def ov2 (find-overlap (nth sample 1) (nth sample 4) 12))
(def ov3 (find-overlap (nth sample 4) (nth sample 1) 12))
(shift-to (:delta ov1) (rotate-each ov1 (:delta ov2))) ; #{[-20 -1133 1061]}
(shift-to (:delta ov1) (rotate-each ov1 (:delta ov3))) ; #{[1172 -1334 -156]}
(:delta ov2) ; [88 113 -1104] :y 1 :z 1
(:delta ov3) ; [-1104 -88 113] :y 3 :z 3
(first (rotate-each ov3 (:delta ov3))) ; [88 -113 -1104]
(def ov4 (find-overlap (nth sample 2) (nth sample 4) 12))
(def ov5 (find-overlap (nth sample 4) (nth sample 2) 12))
(:delta ov4) ; [1125 -168 72]
(:delta ov5) ; [168 -1125 72]
(first (rotate-each ov4 (:delta ov4))) ; [168 -1125 -72]

(map #(dissoc % :common) overlaps)
; ({:delta [68 -1246 -43], :x 0, :y 2, :i 0, :j 1}
;  {:delta [160 -1134 -23], :x 0, :i 1, :j 3}
;  {:delta [88 113 -1104], :z 1, :y 1, :i 1, :j 4}
;  {:delta [1125 -168 72], :y 2, :z 3, :i 2, :j 4})

(defn overlap-delta
  [])

(first (filter #(= 1 (:j %)) '({:j 1} {:j 2})))










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

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;



(def norm1 (find-overlap (nth sample 0) (nth sample 1) 12))
(:normalized norm1)
(def norm2 (find-overlap (:normalized norm1) (nth sample 4) 12))
(def norm3 (find-overlap (:normalized norm1) (nth sample 3) 12))
(def norm4 (find-overlap (:normalized norm2) (nth sample 2) 12))

(def normal
  (loop [scanners (rest full)
       normal [{:delta [0 0 0]
                :normalized (first full)}]
       i 0
       used []]
    (prn i)
  (if (empty? scanners)
    (concat used normal)
    (let [groups (group-by #(find-overlap (:normalized (first normal)) % 12) scanners)]
      (recur
        (get groups nil [])
        (concat (rest normal) (filter :delta (map key groups)))
        (inc i)
        (conj used (first normal)))))))

(map :delta normal)
(count (set (mapcat :normalized normal)))

(defn abs [a] (if (< a 0) (- a) a))

(defn manhatten
  [[x1 y1 z1] [x2 y2 z2]]
  (+ (abs (- x1 x2))
     (abs (- y1 y2))
     (abs (- z1 z2))))

(manhatten [1105,-1205,1229] [-92,-2380,-20])

(apply max (for [a (map :delta normal)
      b (map :delta normal)]
  (manhatten a b)))













