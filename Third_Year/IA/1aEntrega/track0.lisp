

;simple environment
(setq track0 (make-Track) )
(setf (Track-env track0)
	(list 	'(nil nil nil nil nil nil nil nil nil nil nil nil nil nil nil nil nil nil)
			'(nil nil nil nil nil t   t   t   t   t   t   t   t   t   nil nil nil nil)
			'(nil nil nil nil nil t   t   t   t   t   t   t   t   t   t   nil nil nil)
			'(nil t   t   t   t   t   t   nil nil nil nil nil t   t   t   t   t   nil)
			'(nil t   t   t   t   t   nil nil nil nil nil nil nil t   t   t   t   nil)
			'(nil t   t   t   t   nil nil nil nil nil nil nil nil nil t   t   t   nil)
			'(nil nil nil nil nil nil nil nil nil nil nil nil nil nil nil nil nil nil)
	)
)
(setf (Track-endpositions track0) (orderlistofcoordinates '((3 15) (4 15) (5 15) (3 16) (4 16) (5 16))) )

(setf (Track-startpos track0) '( (4 1) ) )

(setf (Track-size track0 ) (list (length (Track-env track0 )) 
						 (length (car (Track-env track0)))
					))


