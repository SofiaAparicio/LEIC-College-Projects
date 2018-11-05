(load "datastructures.lisp")
(load "auxfuncs.lisp")


;;; TAI position
(defun make-pos (c l)
  (list c l))
(defun pos-l (pos)
  (first pos))
(defun pos-c (pos)
  (second pos))

;;; TAI acceleration
(defun make-acce (c l)
  (list c l))
(defun acce-l (pos)
  (first pos))
(defun acce-c (pos)
  (second pos))

;;; TAI velocity
(defun make-vel (c l)
  (list c l))
(defun vel-l (pos)
  (first pos))
(defun vel-c (pos)
  (second pos))


;; Solution of phase 1

(defun getTrackContent (pos track)
  (nth (pos-c pos) (nth (pos-l pos) (track-env track))))

;; Pedir 0,4
(defun isObstaclep (pos track)
  "check if the position pos is an obstacle"
  (or (< (pos-l pos) 0) (< (pos-c pos) 0)
      (>= (pos-l pos) (pos-l (track-size track)))
      (>= (pos-c pos) (pos-c (track-size track)))
      (null (getTrackContent pos track))))

;; Pedir 0,4
(defun isGoalp (st)
  "check if st is a solution of the problem"
  (let ((current-position (state-pos st))
	(track (state-track st)))
    (and (member current-position (track-endpositions track) :test #'equalp)
	 T)))

;; Pedir 1,2
(defun nextState (st act)
  "generate the nextState after state st and action act from prolem"
  (let ((new-state (make-state :action act :track (state-track st))))
    (setf (state-vel new-state)
	  (make-vel (+ (vel-l (state-vel st)) (acce-l act))
		    (+ (vel-c (state-vel st)) (acce-c act))))
    (setf (state-pos new-state)
	  (make-pos (+ (pos-l (state-pos st)) (vel-l (state-vel new-state)))
		    (+ (pos-c (state-pos st)) (vel-c (state-vel new-state)))))
    (setf (state-cost new-state)
	  (cond ((isGoalp new-state) -100)
		((isObstaclep (state-pos new-state) (state-track new-state)) 20)
		(T 1)))
    (when (= (state-cost new-state) 20)
      (setf (state-vel new-state) (make-vel 0 0))
      (setf (state-pos new-state) (make-pos (pos-l (state-pos st))
					    (pos-c (state-pos st)))))
    (values new-state)))

;; Solution of phase 2

;;; Pedir
(defun nextStates (st)
  "generate all possible next states"
  (let (
        (actionsList (possible-actions ))
        (statesList (list ))
        )
    (dolist (el actionsList statesList)
      (push (nextState st el) statesList)
    )
  )
)

;;; limdepthfirstsearch
(defun limdepthfirstsearch (problem lim)
  "limited depth first search
     st - initial state
     problem - problem information
     lim - depth limit"
	(list (make-node :state (problem-initial-state problem))) )


;iterlimdepthfirstsearch
(defun iterlimdepthfirstsearch (problem)
  "limited depth first search
    st - initial state
     problem - problem information
     lim - limit of depth iterations"
	(list (make-node :state (problem-initial-state problem))) )

;; Solution of phase 3

;; Heuristic
(defun compute-heuristic (st)
	(cond
    ((isGoalp st) (return-from compute-heuristic 0))
    ((isObstaclep (state-pos st) (state-track st)) (return-from compute-heuristic most-positive-fixnum))
  )
  (let* ((goalPositions (track-endpositions (state-track st)))
         (min_path most-positive-fixnum)
         (distance 0)
         (statePos (state-pos st))
        )
       (dolist (goal goalPositions)
          (if (> (- (pos-l goal) (pos-l statePos)) (- (pos-c goal) (pos-c statePos)))
            (setf distance (- (pos-l goal) (pos-l statePos)))
            (setf distance (- (pos-c goal) (pos-c statePos)))
          )
          (if (< distance min_path)
            (setf min_path distance)
            nil
          )
        )
  (return-from compute-heuristic min_path)
  )
)



;;; A*
(defun a* (problem)
  (let* ( (initial_state (problem-initial-state problem))
          (h_value (compute-heuristic initial_state))
          (queue (make-heap :nodes (list '() (make-node :state initial_state :f h_value  :g 0 :h h_value))) :size 1)
        )
        (loop while ( not( equal(heap-size queue) 0))
          do(let ((minNode (extractMin queue))
                (children (nextState (node-state minNode)))
               )
            (dolist (child children)
              (let* ( (h_child (compute-heuristic child))
                      (g_child (+ (state-cost child) (node-g minNode)))
                      (childNode (make-node :state child :parent minNode :f (+ h_child g_child) :g g_child :h h_child))
                    )
               (when  (funcall (problem-fn-isGoal problem) state)
                    (return-from a* (solution childNode))
               )
               (insert childNode)
              )
            )
          )
        )
      )
  )



(defun extractMin1 (queue)
  (let ((minNode (first queue))
       )
    (dolist (el queue minNode)
      (if (< (node-f el) (node-f minNode))
        (setf minNode el)
        nil
      )
    )
  )
)

;;;;------------------------- HEAP --------------------
;;;raiz da heap comeca no 1
(defstruct heap
  nodes
  size)

(defun insert (node queue)
  (setf (heap-size queue) (1+ (heap-size queue)))
  (let* ((i (heap-size queue))
        (parent (truncate i 2))
       )

    (setf (heap-nodes queue) (append (heap-nodes queue) (list node)))
    ;;;(print (heap-nodes queue))
    (loop while (and (> i 1) (> (node-f (nth parent (heap-nodes queue))) (node-f (nth i (heap-nodes queue)))))
      do (swap i parent queue)
         (setf i parent)
         (setf parent (truncate i 2))
    )
  )
)

(defun extractMin (queue)
  (let ((minNode (second (heap-nodes queue)))
       )
    ;;;(setf (second (heap-nodes queue)) (nth (heap-size queue) (heap-nodes queue)))
    (swap 1 (heap-size queue) queue)
    (setf (heap-size queue) (1- (heap-size queue)))
    (delete minNode (heap-nodes queue))
    ;;(print (heap-nodes queue))
    (orderDown queue 1)
    minNode
  )
)

(defun orderDown (queue index)
  (let ((left (* 2 index))
        (right (1+ (* 2 index)))
        (smallest index)
        (size (heap-size queue))
        (nodes (heap-nodes queue))
       )
    (if (and (<= left size) (< (node-f (nth left nodes)) (node-f (nth smallest nodes))))
      (setf smallest left))

    (if (and (<= right size) (< (node-f (nth right nodes)) (node-f (nth smallest nodes))))
      (setf smallest right)
    )

    (when (not (equal smallest index))
      (swap smallest index queue)
      (orderDown queue smallest)
    )
  )
)

(defun swap (i1 i2 queue)
  (let ((temp (nth i1 (heap-nodes queue)))
       )
    (setf (nth i1 (heap-nodes queue)) (nth i2 (heap-nodes queue)))
    (setf (nth i2 (heap-nodes queue)) temp)
  )
)


;;; Solucao e uma seq ordenada de estados
(defun solution (node)
  (let ((seq-states nil))
    (loop
      (when (null node)
	(return))
      (push (node-state node) seq-states)
      (setf node (node-parent node)))
    (values seq-states)))
