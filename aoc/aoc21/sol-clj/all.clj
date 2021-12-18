(ns aoc21)
(require '[clojure.edn :as edn])
(require '[clojure.string :as string])

;; Helpers ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn abs
  [n]
  (if (> n 0)
    n
    (- n)))


;;; Day 1 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn day1-read
  [text-data]
  (map #(Integer/parseInt %) (string/split text-data #"\n")))

(def day1-sample (day1-read (slurp "data/day1.sample")))
(def day1-full (day1-read (slurp "data/day1.full")))


;; Solve A
(defn day1-a
  "Find the number of depths that are greater than the previous depth"
  [depths]
  (reduce
    (fn [[acc lst] x]
      [(if (> x lst) (inc acc) acc)
       x])
    [0, 10000000000]
    depths))

;; A answers
(day1-a day1-sample) ; [7 263]
(day1-a day1-full) ; [1184 2682]


;; Helpers B
(defn window-sum
  "Returns a vec of sums for each full window size in a given seq of nums"
  [nums size]
  (loop [low 0
         high size
         sums [(reduce + (take size nums))]]
    (if (nth nums high nil)
      (recur
        (inc low)
        (inc high)
        (conj sums
              (-> (peek sums)
                  (- (nth nums low))
                  (+ (nth nums high)))))
      sums)))

(window-sum day1-sample 3)
; [607 618 618 617 647 716 769 792]

;; Solve B
(defn day1-b
  "Create a list of sums of size 3 from depths"
  [depths]
  (#'day1-a (#'window-sum depths 3)))

;; B answers
(day1-b day1-sample) ; [5 792]
(day1-b day1-full) ; [1158 8013]


;;; Day 2 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(def day2-sample (slurp "data/day2.sample"))
(def day2-data (slurp "data/day2.full"))

(defn day2-a)

(defn day2-b)


;;; Day 3 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn day3-read
  [text-data]
  (map
    (fn [x] (map #(Integer/parseInt %) (string/split x #"")))
    (string/split text-data #"\n")))

(def day3-sample (day3-read (slurp "data/day3.sample")))
(def day3-full (day3-read (slurp "data/day3.full")))


;; Helpers A
(defn transpose [M] (apply mapv vector M))
(transpose day3-sample)
; [[0 1 1 1 1 0 0 1 1 1 0 0]
;  [0 1 0 0 0 1 0 1 0 1 0 1]
;  [1 1 1 1 1 1 1 1 0 0 0 0]
;  [0 1 1 1 0 1 1 0 0 0 1 1]
;  [0 0 0 1 1 1 1 0 0 1 0 0]]

(defn row-frequencies [M] (map #(frequencies %) M))
(def freqs (row-frequencies (transpose day3-sample)))
; ({0 5, 1 7} {0 7, 1 5} {1 8, 0 4} {0 5, 1 7} {0 7, 1 5})

(defn most-common-bits [freqs] (map #(if (> (get % 0 0) (get % 1 0)) 0 1) freqs))
(most-common-bits freqs) ; (1 0 1 1 0)

(defn flip [bit] (mod (+ 1 bit) 2))
(flip 0) ; 1
(flip 1) ; 0

(defn least-common-bits [freqs] (map #'flip (#'most-common-bits freqs)))
(least-common-bits freqs) ; (0 1 0 0 1)

(defn join-as-decimal
  [bit-list]
  (edn/read-string
    (str "2r" (string/join "" bit-list))))
(join-as-decimal (most-common-bits freqs)) ; 22
(join-as-decimal (least-common-bits freqs)) ; 9

;; Solve A 
(defn day3-a
  "Calculate the power consumption as directed"
  [data]
  (let [freqs (-> data transpose row-frequencies)]
    (* (#'join-as-decimal (#'most-common-bits freqs))
       (#'join-as-decimal (#'least-common-bits freqs)))))

;; Solve A
(day3-a day3-sample) ; 198
(day3-a day3-full) ; 738234


;; Helpers B
(defn have-nth-bit
  [data bit idx]
  (filter #(= bit (nth % idx)) data))
(have-nth-bit day3-sample 1 0)
; ((1 1 1 1 0)
;  (1 0 1 1 0)
;  (1 0 1 1 1)
;  (1 0 1 0 1)
;  (1 1 1 0 0)
;  (1 0 0 0 0)
;  (1 1 0 0 1))

(defn common-bit
  [data idx bit-func]
  (nth
    (-> data transpose row-frequencies bit-func)
    idx))
(defn mcb [data idx] (common-bit data idx most-common-bits))
(defn lcb [data idx] (common-bit data idx least-common-bits))
(mcb day3-sample 0) ; 1
(lcb day3-sample 0) ; 0

(defn find-rating
  [data bit-func]
  (first
    (reduce
      (fn [acc idx]
        (if (> (count acc) 1)
          (#'have-nth-bit acc (bit-func acc idx) idx)
          acc))
      data
      (range (count (first data))))))
(find-rating day3-sample mcb) ; (1 0 1 1 1)
(find-rating day3-sample lcb) ; (0 1 0 1 0)


;; Solve B
(defn day3-b
  "Calculate the life support rating as directed"
  [data]
  (* (#'join-as-decimal (#'find-rating data #'mcb))
     (#'join-as-decimal (#'find-rating data #'lcb))))

;; B answers
(day3-b day3-sample) ; 230
(day3-b day3-full) ; 3969126


;;; Day 4 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn read-int-line
  [line]
  (mapv #(Integer/parseInt %) (string/split (string/trim line) #" +")))

(read-int-line "  2   3 4 5 6  ") ; [2 3 4 5 6]

(defn day4-read-board
  "Read the next n+1 lines into a single vector of numbers representing
   a bingo board. Return the board and the rest of the lines."
  [lines n]
  {:board (reduce
            (fn [acc x]
              (concat acc (read-int-line x)))
            []
            (take n lines))
   :lines (drop (inc n) lines)})

(day4-read-board ["1 2" "  3 4  " "" "5 6" " 7  9 " ""] 2)
; {:board (1 2 3 4), :lines ("5 6" " 7  9" "")}

(defn day4-read-boards
  "Read all boards of size n x n into a vector of 1d vectors
   representing boards"
  [lines n]
  (loop [rest-lines lines
         boards []]
    (if (empty? rest-lines)
      boards
      (let [bb (day4-read-board rest-lines n)]
        (recur (:lines bb) (conj boards (:board bb)))))))

(day4-read-boards ["1 2" "3  4" "" "5 6" " 7  9" ""] 2) ; [(1 2 3 4) (5 6 7 9)]

(defn day4-read
  [text-data]
  (let [lines (string/split text-data #"\n")]
        {:nums (mapv #(Integer/parseInt %)
                  (string/split (first lines) #","))
         :boards (day4-read-boards (drop 2 lines) 5)}))

(def day4-sample (day4-read (slurp "data/day4.sample")))
(def day4-full (day4-read (slurp "data/day4.full")))

;; Helpers A
(defn mark-boards
  [boards x]
  (mapv (fn [b]
          (mapv #(if (= x %) [x] %) b))
        boards))
(mark-boards (:boards day4-sample) 2)
; [[22 13 17 11 0 8 [2] 23 4 24 21 9 14 16 7 6 10 3 18 5 1 12 20 15 19]
;  [3 15 0 [2] 22 9 18 13 17 5 19 8 7 25 23 20 11 10 24 4 14 21 16 12 6]
;  [14 21 17 24 4 10 16 15 9 19 18 8 23 26 20 22 11 13 6 5 [2] 0 12 3 7]]

(defn row-win?
  [board n]
  (loop [b board]
    (cond
      (empty? b) false
      (every? #(not (int? %)) (take n b)) true
      :else (recur (drop n b)))))

(row-win? [1 2 3 4] 2) ; false
(row-win? [1 2 [3] [4]] 2) ; true

(defn col-win?
  [board n]
  (row-win? (mapcat identity (transpose (partition n n board))) n))

(col-win? [1 [2] 3 [4]] 2) ; true
(col-win? [1 2 [3] [4]] 2) ; false

(defn get-diagonals
  [board n]
  (loop [b board
         i 0
         left []
         right []]
    (if (empty? b)
      {:left left :right right}
      (recur (drop n b) (inc i)
             (conj left (nth b i))
             (conj right (nth b (- (dec n) i)))))))

(get-diagonals [1 2 3 4 5 6 7 8 9] 3) ; {:left [1 5 9], :right [3 5 7]}

(defn diagonal-win?
  [board n]
  (let [diagonals (get-diagonals board n)]
    (or (row-win? (:left diagonals) n)
        (row-win? (:right diagonals) n))))

(diagonal-win? [[1] 2 3 4 [5] 6 7 8 [9]] 3) ; true
(diagonal-win? [1 2 [3] 4 [5] 6 [7] 8 9] 3) ; true
(diagonal-win? [1 2 3 4 [5] 6 [7] 8 9] 3) ; false
(diagonal-win? [1 2 3 4 5 6 7 8 9] 3) ; false

(defn winner?
  [board n]
  (or (row-win? board n)
      (col-win? board n)))
      ;;(diagonal-win? board n)))

(defn find-first-winner
  "Marks boards for each number and exits early if a winner has been found.
   Assumes that there will always be a board that can win."
  [boards nums n]
  (loop [new-boards boards
         called -1
         new-nums nums]
    (let [winner (filter #(winner? % n) new-boards)]
      (if (seq winner)
        {:winner (first winner) :called called}
        (recur
          (mark-boards new-boards (first new-nums))
          (first new-nums)
          (rest new-nums))))))

;; Solve A
(defn day4-a
  [data]
  (let [winner (find-first-winner (:boards data) (:nums data) 5)
        not-marked (filter int? (:winner winner))]
    (* (:called winner) (reduce + not-marked))))

;; Answers A
(day4-a day4-sample) ; 4512
(day4-a day4-full) ; 82440


;; Helpers B
(defn find-last-winner
  "Marks boards for each number and filters until there is only one board left.
   Assumes that all boards will win eventually."
  [boards nums n]
  (loop [new-boards boards
         called -1
         new-nums nums]
    (let [not-winners (filter #(not (winner? % n)) new-boards)]
      (if (= 1 (count not-winners))
        (find-first-winner not-winners nums n)
        (recur
          (mark-boards not-winners (first new-nums))
          (first new-nums)
          (rest new-nums))))))

;; Solve B
(defn day4-b
  [data]
  (let [winner (find-last-winner (:boards data) (:nums data) 5)
        not-marked (filter int? (:winner winner))]
    (* (:called winner) (reduce + not-marked))))

(day4-b day4-sample) ; 1924
(day4-b day4-full) ; 20774


;;; Day 5 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn day5-read
  [text-data]
  (->> (string/split text-data #"\n")
       (map #(string/replace % "," " "))
       (map #(string/replace % #"->" " "))
       (map read-int-line)))

(def day5-sample (day5-read (slurp "data/day5.sample")))
(def day5-full (day5-read (slurp "data/day5.full")))

;; Helpers A
(defn same-sign
  [a b]
  (if (or (and (< a 0) (>= b 0))
          (and (>= a 0) (< b 0)))
    (- b)
    b))

(defn reduced-frac
  [a b]
  (let [r (/ a b)]
    (cond
      (int? r) [(same-sign a r) (same-sign b 1)]
      (and (neg? a) (neg? b)) [(- (numerator r)) (- (denominator r))]
      :else [(numerator r) (denominator r)])))

(defn slope
  [x1 y1 x2 y2]
  (let [dy (- y2 y1)
        dx (- x2 x1)]
    (if (= 0 dx)
      [(cond (< dy 0) -1 (> dy 0) 1 :else 0) 0]
      (reduced-frac dy dx))))

(slope 0 0 0 5) ; [1 0]
(slope 0 0 2 2) ; [1 1]
(slope 4 6 2 2) ; [-2 -1]
(slope 0 6 2 2) ; [-2 1]
(slope 6 8 2 2) ; [-3N -2N]

(defn gen-points
  [[x1 y1 x2 y2]]
  (let [start [x1 y1]
        end [x2 y2]
        [dy dx] (slope x1 y1 x2 y2)]
    (loop [[px py] start
           points []]
      (if (= [px py] end)
        (conj points [px py])
        (recur
          [(+ px dx) (+ py dy)]
          (conj points [px py]))))))

(gen-points [0 0 0 5]) ; [[0 0] [0 1] [0 2] [0 3] [0 4] [0 5]]
(gen-points [0 0 2 2]) ; [[0 0] [1 1] [2 2]]
(gen-points [4 6 2 2]) ; [[4 6] [3 4] [2 2]]

(defn no-diagonals
  [lines]
  (filter
    (fn [[x1 y1 x2 y2]]
      (or (= x1 x2) (= y1 y2)))
    lines))

;; Solve A
(defn day5-a
  [data n]
  (->> data
       no-diagonals
       (mapcat gen-points)
       frequencies
       (map val)
       (filter #(>= % n))
       count))

(day5-a day5-sample 2) ; 5
(day5-a day5-full 2) ; 6189

;; Solve B
(defn day5-b
  [data n]
  (->> data
       (mapcat gen-points)
       frequencies
       (map val)
       (filter #(>= % n))
       count))

(day5-b day5-sample 2) ; 12
(day5-b day5-full 2) ; 19164


;;; Day 6 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(def day6-sample [3 4 3 1 2])
(def day6-full [3,3,5,1,1,3,4,2,3,4,3,1,1,3,3,1,5,4,4,1,4,1,1,1,3,3,2,3,3,4,2,5,1,4,1,2,2,4,2,5,1,2,2,1,1,1,1,4,5,4,3,1,4,4,4,5,1,1,4,3,4,2,1,1,1,1,5,2,1,4,2,4,2,5,5,5,3,3,5,4,5,1,1,5,5,5,2,1,3,1,1,2,2,2,2,1,1,2,1,5,1,2,1,2,5,5,2,1,1,4,2,1,4,2,1,1,1,4,2,5,1,5,1,1,3,1,4,3,1,3,2,1,3,1,4,1,2,1,5,1,2,1,4,4,1,3,1,1,1,1,1,5,2,1,5,5,5,3,3,1,2,4,3,2,2,2,2,2,4,3,4,4,4,1,2,2,3,1,1,4,1,1,1,2,1,4,2,1,2,1,1,2,1,5,1,1,3,1,4,3,2,1,1,1,5,4,1,2,5,2,2,1,1,1,1,2,3,3,2,5,1,2,1,2,3,4,3,2,1,1,2,4,3,3,1,1,2,5,1,3,3,4,2,3,1,2,1,4,3,2,2,1,1,2,1,4,2,4,1,4,1,4,4,1,4,4,5,4,1,1,1,3,1,1,1,4,3,5,1,1,1,3,4,1,1,4,3,1,4,1,1,5,1,2,2,5,5,2,1,5])

;; Helpers A
(defn gen-fish
  [s t r]
  (range (inc s) (inc t) r))
(gen-fish 3 18 7)

(def growth-memo
  (memoize
    (fn [s t r a]
      (if (= 0 t)
        1
        (+ 1
           (reduce +
                   (map #(growth-memo (+ r (dec a)) (- t %) r a)
                        (gen-fish s t r))))))))

(defn all-growth
  [t r a]
  (map #(growth-memo % t r a) (range r)))

(def after-80 (all-growth 80 7 2))

;; Solve A
(defn day6-a
  [data]
  (reduce +
          (map #(* (nth after-80 (first %))
                   (second %))
               (frequencies data))))
  

(day6-a day6-sample) ; 5934
(day6-a day6-full) ; 362740


;; Solve B
(def after-256 (all-growth 256 7 2))

(defn day6-b
  [data]
  (reduce +
          (map #(* (nth after-256 (first %))
                   (second %))
               (frequencies data))))

(day6-b day6-sample) ; 26984457539
(time (day6-b day6-full)) ; 1644874076764



;;; Day 7 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(def day7-sample [16,1,2,0,4,2,7,1,2,14])
(def day7-full [1101,1,29,67,1102,0,1,65,1008,65,35,66,1005,66,28,1,67,65,20,4,0,1001,65,1,65,1106,0,8,99,35,67,101,99,105,32,110,39,101,115,116,32,112,97,115,32,117,110,101,32,105,110,116,99,111,100,101,32,112,114,111,103,114,97,109,10,104,709,235,932,796,119,731,120,462,837,152,100,214,411,71,28,91,1231,401,417,900,1733,683,107,101,582,159,72,11,514,566,1054,638,774,413,222,568,526,53,303,635,664,21,67,133,913,292,95,963,7,440,78,1455,283,104,106,431,749,468,325,319,922,433,2,108,10,95,89,1074,190,91,52,1313,242,475,964,395,437,604,277,525,162,191,923,124,219,35,707,18,1123,30,1163,41,467,290,420,393,279,159,59,206,160,592,52,267,696,218,151,807,301,262,424,102,1871,406,443,149,1035,1286,141,403,37,872,1031,788,1138,962,89,357,885,367,499,175,556,157,1571,759,989,2,1305,38,132,579,335,1452,171,627,175,557,1108,274,263,1036,482,432,21,1769,63,17,731,83,1329,131,101,6,1135,317,110,41,706,142,292,473,783,566,230,34,243,405,32,55,987,646,62,92,52,597,48,319,1159,827,769,125,420,308,60,345,461,159,229,1064,298,1200,861,364,1051,26,584,702,1717,19,61,35,581,297,63,945,1469,3,1168,588,339,1182,1357,823,293,85,77,40,847,235,326,364,474,619,732,105,517,153,32,198,65,1026,278,1170,1092,941,1747,147,124,86,975,856,1173,350,51,206,17,319,111,89,49,94,97,319,887,307,991,372,175,409,359,129,1242,1409,644,205,424,1644,1515,1134,299,571,78,695,101,365,385,1188,1162,17,106,972,198,381,656,9,291,1415,95,1048,541,162,1408,776,308,308,278,495,1679,302,1,138,7,382,981,455,719,607,541,136,449,1059,227,453,1614,315,283,583,143,1806,499,1062,1115,219,22,160,650,326,70,316,4,200,1542,1554,266,377,123,1302,1814,139,383,304,324,167,850,63,306,365,83,490,201,41,352,593,118,45,554,75,1352,49,92,1399,231,104,289,134,1307,9,247,883,999,1069,301,307,743,729,365,3,1251,415,304,40,330,293,72,393,562,12,183,41,229,306,209,281,1557,126,1119,286,12,18,1010,729,741,738,44,615,748,193,598,423,68,174,36,70,1455,325,0,229,409,211,423,183,271,233,952,601,320,109,1051,502,684,546,239,1279,215,1497,125,427,489,500,10,415,189,630,261,63,102,1459,79,1113,199,684,251,801,573,16,99,1805,716,45,18,631,290,508,67,0,2,461,63,325,607,697,812,58,262,316,754,37,848,60,101,202,1000,128,20,355,313,140,279,833,168,1197,1668,1062,255,626,205,326,321,591,243,1093,38,26,986,508,424,229,143,163,1173,608,349,468,571,95,140,10,279,112,12,552,0,326,258,195,113,470,651,1298,439,53,134,151,447,299,905,40,19,23,719,10,557,1339,474,119,329,1487,55,602,255,284,162,783,524,452,899,327,236,1826,295,265,598,1825,220,517,592,862,57,762,465,313,499,694,1328,5,81,137,936,46,852,448,1301,1101,35,77,1283,11,193,937,757,9,208,160,736,54,1574,87,546,51,373,29,25,79,1091,1432,125,158,728,835,1,614,172,389,173,808,1788,223,125,135,25,318,6,691,724,104,467,269,66,39,362,155,100,165,425,1844,41,284,602,226,294,172,942,223,1,14,199,1292,235,434,612,980,139,61,735,276,62,864,56,460,652,713,98,408,1314,320,116,171,114,93,804,260,339,451,392,31,156,176,60,279,1272,271,1494,164,170,451,857,317,1379,44,166,115,823,349,4,352,54,389,1548,302,454,1412,231,86,2,239,117,272,462,1030,171,14,301,249,66,114,360,676,510,1149,58,91,46,317,425,1219,64,1538,638,1227,62,214,386,1148,180,327,1084,27,886,565,157,215,313,462,129,1293,397,823,753,50,539,705,813,531,779,30,501,1072,1125,2,1640,691,1140,573,1081,1232,488,721,113,113,127,270,1095,6,68,301,465,43,322,88,892,841,323,981,642,1231,346,247,623,161,1291,76,709,1148,306,87,1147,645,818,1520,692,352,133,71,443,1190,271,1171,42,980,589,493,312,211,78,1369,329,304,1057,202,405,1294,49,363,835,1295,53,530,20,24,947,885,1054,252,1170,337,460,476,50,657,1201,715,555,132,344,26,1369,675,234,1362,875,224,1910,338,175,93,595,27,211,210,787,790,990,425,1176,48,43,201,15,279,344,203,15,790,255,125,159,45,162,290,198,796,52,146,512,200,1051,1850,1202,775,237,767,13,180,294,26,896,1263,749,1239,1621,642,607,88,123,651,630,1178,135,5,686,989,1250,60,1266,360,49,1089,175,355,162,375,350,1203])

;; Solve A

;; More efficient way to do part A is to use the median element as the best
;; place to align to and then just calculate 1 cost. This works because each
;; position you move to left or right changes the cost of all items by +/- 1.
;; So the best place to be is where there are half on one side and half on
;; the other. Likely when there are 2 choices you have to try both.

(defn align
  [pos dest]
  (abs (- pos dest)))

(defn fuel-cost
  [data dest align]
  (reduce + (map #(align % dest) data)))
(time (fuel-cost day7-full 2 align))

(defn best-align
  [data align]
  (let [mn (apply min data)
        mx (apply max data)]
    (apply min
           (map #(fuel-cost data % align)
                (range mn mx)))))

;; Answers A
(best-align day7-sample align) ; 37
(time (best-align day7-full align)) ; 335271
(time (mapv #(fuel-cost day7-full % align) (range 2000)))


;; Solve B

;; Not sure of the efficient way of doing this one. If the align cost was
;; distance squared the best middle changes to the mean, but with sum of n
;; it is not doable that way.

(defn sum-series-n
  [n]
  (/ (* n (+ n 1)) 2))

(defn align-increasing
  [pos dest]
  (sum-series-n (align pos dest)))

;; Answers B
(best-align day7-sample align-increasing) ; 168
(time (best-align day7-full align-increasing)) ; 95851339


;;; Day 8 ;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
(defn day8-read-item
  [text-data]
  (let  [[input output] (string/split text-data #"\|")]
    {:input (string/split (string/trim input) #" ")
     :output (string/split (string/trim output) #" ")}))

(defn day8-read
  [text-data]
  (map day8-read-item (string/split text-data #"\n")))

(def day8-sample (day8-read (slurp "data/day8.sample")))
(def day8-full (day8-read (slurp "data/day8.full")))

(defn day8-a
  [data]
  (apply
    +
    (map
      (fn [d]
        (count (filter #{2 3 4 7} (map count d))))
      (map :output data))))

(day8-a day8-sample) ; 26
(day8-a day8-full) ; 330

;;; Part B

;; Figure out which pattern is which number

(defn could-be?
  [config]
  (case (count config)
    2 #{1}
    3 #{7}
    4 #{4}
    5 #{2 3 5}
    6 #{6 9 0}
    7 #{8}))

(could-be? "ab") ; #{1}
(could-be? "abc") ; #{7}
(could-be? "abcd") ; #{4}
(could-be? "abcde") ; #{3 2 5}
(could-be? "abcdef") ; #{0 6 9}
(could-be? "abcdefg") ; #{8}

(def default-configs
  {1 #{\c \f}
   2 #{\a \c \d \e \g}
   3 #{\a \c \d \f \g}
   4 #{\b \c \d \f}
   5 #{\a \b \d \f \g}
   6 #{\a \b \d \e \f \g}
   7 #{\a \c \f}
   8 #{\a \b \c \d \e \f \g}
   9 #{\a \b \c \d \f \g}
   0 #{\a \b \c \e \f \g}})

(def default-patterns
  (reduce (fn [acc x]
            (assoc acc
                   (key x)
                   (apply str (into [] (val x)))))
          {}
          default-configs))

(defn intersects?
  [pattern config size]
  (= size
     (count (clojure.set/intersection (set pattern)
                                      config))))

;; size is 5
(defn is-3?
  "Note only good for disambiguating 3 from 2 and 5"
  [pattern configs]
  (if (contains? (could-be? pattern) 3)
    (intersects? pattern (get configs 1) 2)
    false))

(is-3? (get default-patterns 3) default-configs) ; true
(is-3? (get default-patterns 2) default-configs) ; false
(is-3? (get default-patterns 5) default-configs) ; false

(defn is-5?
  "Note only good for disambiguating 5 from 2"
  [pattern configs]
  (if (contains? (could-be? pattern) 3)
    (intersects? pattern (get configs 4) 3)
    false))

(is-5? (get default-patterns 5) default-configs) ; true
(is-5? (get default-patterns 2) default-configs) ; false

;;; if it is of size 3 and not a 3 or 5 then it is a 2

;; size is 6
(defn is-9?
  "Note only good for disambiguating 9 from 6 and 0"
  [pattern configs]
  (if (contains? (could-be? pattern) 6)
    (intersects? pattern (get configs 4) 4)
    false))

(is-9? (get default-patterns 9) default-configs) ; true
(is-9? (get default-patterns 6) default-configs) ; false
(is-9? (get default-patterns 0) default-configs) ; false

(defn is-6?
  "Note only good for disambiguating 6 from 0"
  [pattern configs]
  (if (contains? (could-be? pattern) 6)
    (intersects? pattern (get configs 7) 2)
    false))

(is-6? (get default-patterns 6) default-configs) ; true
(is-6? (get default-patterns 0) default-configs) ; false

;;; If it is size 6 and not 6 or 9 then it is 0

;; Generate the configurations that know which set of wires makes up each number

(defn group-could-be
  [entry]
  (group-by could-be? (:input entry)))

(defn get-initial-configs
  [groups]
  (reduce
   (fn [acc x]
     (if (= 1 (count (key x)))
       (assoc acc
              (first (vec (key x)))
              (set (first (val x))))
       acc))
   {}
   groups))

(get-initial-configs (group-could-be (first day8-sample)))
; {1 #{\b \e}, 8 #{\a \b \c \d \e \f \g}, 4 #{\b \c \e \g}, 7 #{\b \d \e}}

(defn get-size-5-configs
  [groups configs]
  (let [group-by-3 (group-by #(is-3? % configs) (get groups #{2 3 5}))
        group-by-5 (group-by #(is-5? % configs) (get group-by-3 false))]
    (assoc configs
           3 (set (first (get group-by-3 true)))
           5 (set (first (get group-by-5 true)))
           2 (set (first (get group-by-5 false))))))

(defn get-size-6-configs
  [groups configs]
  (let [group-by-9 (group-by #(is-9? % configs) (get groups #{6 9 0}))
        group-by-6 (group-by #(is-6? % configs) (get group-by-9 false))]
    (assoc configs
           9 (set (first (get group-by-9 true)))
           6 (set (first (get group-by-6 true)))
           0 (set (first (get group-by-6 false))))))

(defn all-configs
  [entry]
  (let [groups (group-could-be entry)
        initial-configs (get-initial-configs groups)]
    (->> initial-configs
         (get-size-5-configs groups)
         (get-size-6-configs groups))))

(def entry (day8-read-item "acedgfb cdfbe gcdfa fbcad dab cefabd cdfgeb eafb cagedb ab | cdfeb fcadb cdfeb cdbaf"))
(all-configs entry)
; {0 #{\a \b \c \d \e \g},
;  7 #{\a \b \d},
;  1 #{\a \b},
;  4 #{\a \b \e \f},
;  6 #{\b \c \d \e \f \g},
;  3 #{\a \b \c \d \f},
;  2 #{\a \c \d \f \g},
;  9 #{\a \b \c \d \e \f},
;  5 #{\b \c \d \e \f},
;  8 #{\a \b \c \d \e \f \g}}

       
;; Solve B

(defn decode-output
  [entry configs]
  (Integer/parseInt
    (apply str
      (map #(get (clojure.set/map-invert configs) %)
           (map set (:output entry))))))

(decode-output entry (all-configs entry)) ; 5353

(defn day8-b
  [data]
  (apply + (map #(decode-output % (all-configs %)) data)))

(day8-b day8-sample) ; 61229
(day8-b day8-full) ; 1010472


