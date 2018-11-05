;;; 81105 Ana Sofia Costa, 81478 Joana Mira Godinho, Grupo 93
;;; These functions, and any other ones needed must be implemented

;;; Utilizar estes includes para os testes na versao local
;;; comentar antes de submeter
;;;(load "datastructures.lisp")
;;;(load "auxfuncs.lisp")

;;; Utilizar estes includes para a versao a submeter
; tirar o comentario antes de submeter
(load "datastructures.fas")
(load "auxfuncs.fas")

(defun isObstaclep (pos track)
  "check if there is an obstacle at position pos of the track"
  (let ((pista (track-env track)))
	  (if (nth (cadr pos) (nth (car pos) pista))  ;;;cadr porque so cdr devolve (2) e nao 2 por exemplo
	  		Nil
	  		t)
	)
  )

(defun isGoalp (st)
  "check if st is a goal state"
  (let ((lista (track-endpositions (state-track st))))
  	(dolist (el lista nil)
  		(when (equal el (state-pos st)) (return t)))
  	)
 )

(defun nextState (st act)
  "generate the nextState after state st and action act"
  (let* ((prevPos (state-pos st))
  		 (prevSpeed (state-vel st))
       (newPosx (+ (first prevPos) (first prevSpeed) (first act)))
   		  (newPosy (+ (second prevPos) (second prevSpeed) (second act)))
   			(newSpeedx (+ (first prevSpeed) (first act)))
   			 (newSpeedy (+ (second prevSpeed) (second act)))
         (Pos (list newPosx newPosy))
         (newState (make-STATE :POS Pos
                         :VEL (list newSpeedx newSpeedy)
                         :ACTION act
                         :COST 0
                         :TRACK (state-track st)))
  		 )


       (cond ((isObstaclep Pos (state-track st))
           (setf (state-COST newState) 20))
           ((isGoalp newState)
             (setf (state-COST newState) -100))
           (T
             (setf (state-COST newState) 1))
       )
       newState
  	)
)
