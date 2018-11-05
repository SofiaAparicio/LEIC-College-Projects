;;; 81105 Ana Sofia Costa, 81478 Joana Mira Godinho, Grupo 93

(load "datastructures.lisp")
(load "auxfuncs.lisp")

;;; Utilizar estes includes para a versao a submeter
; tirar o comentario antes de submeter
;(load "datastructures.fas")
;(load "auxfuncs.fas")

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

(defun recursiveDfs (state problem limit)
  (cond ((funcall (problem-fn-isGoal problem) state)
           (return-from recursiveDfs (list state)))
        ((equal limit 0) (return-from recursiveDfs ':corte))
  )
  (let (
        (cutoff_occurred nil)
        (childStates (funcall (problem-fn-nextStates problem) state))
       )
    (dolist (child childStates)
      (let*(
            (result (recursiveDfs child problem (- limit 1)))
          )
        (cond ((equal result ':corte) (setf cutoff_occurred T))
              ((not (equal result nil)) (return-from recursiveDfs (append (list state) result)))
        )
      )
    )
    (if cutoff_occurred
        ':corte
        nil
    )
  )
)

;;; limdepthfirstsearch
(defun limdepthfirstsearch (problem lim)
  "limited depth first search"
     (recursiveDfs (problem-initial-state problem) problem lim)
  )


;iterlimdepthfirstsearch
(defun iterlimdepthfirstsearch (problem)
  "limited depth first search"
	(let (
			(lim 0)
      (result nil)
		)

		(loop
    		(setf result (limdepthfirstsearch problem lim))
    		(setf lim (+ lim 1))
    		(if (not (equal result ':corte))
    			(return-from iterlimdepthfirstsearch result)
    		)
    	)
    )
)
