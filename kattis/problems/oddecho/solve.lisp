
(defun solve (data)
  (loop for s in data and i from 0 do
        (when (equal 0 (mod i 2))
          (format t "~a~%" s))))

;(solve '("hello" "world" "you"))


;; Read data and run solution
(loop for n = (read *standard-input* nil) until (null n) do
      (solve (loop repeat n
                   collect (read))))

