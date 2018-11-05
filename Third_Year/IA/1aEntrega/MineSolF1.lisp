
;;; These functions, and any other ones needed must be implemented

(load "datastructures.lisp")
(load "auxfuncs.lisp")

(defun isObstaclep (pos track)
;;;check if there is an obstacle at position pos of the track"   nth coluna (nth linha n)
  (let ( (linha (car pos)) (coluna (cadr pos)) (caminho (track-env track)) )
    (not (nth coluna (nth linha caminho)))
  )
)

(defun isGoalp (st)
;;;"check if st is a goal state"
  (let ( (position (state-pos st)) (endpositions (Track-endpositions (state-track st))) )

    (loop for x in endpositions
      if (equal x position)
        do(return t)
    )
  )
)

(defun nextState (st act)
;;;"generate the nextState after state st and action act"
  (let ( (position (state-pos st)) (velocity (state-vel st)) (cost (state-cost st)) )
    (make-STATE :POS '(3 16)
          :VEL '(1 3)
          :ACTION act
          :COST -100)

  )
)
