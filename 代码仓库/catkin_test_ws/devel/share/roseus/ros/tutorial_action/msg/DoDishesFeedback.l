;; Auto-generated. Do not edit!


(when (boundp 'tutorial_action::DoDishesFeedback)
  (if (not (find-package "TUTORIAL_ACTION"))
    (make-package "TUTORIAL_ACTION"))
  (shadow 'DoDishesFeedback (find-package "TUTORIAL_ACTION")))
(unless (find-package "TUTORIAL_ACTION::DODISHESFEEDBACK")
  (make-package "TUTORIAL_ACTION::DODISHESFEEDBACK"))

(in-package "ROS")
;;//! \htmlinclude DoDishesFeedback.msg.html


(defclass tutorial_action::DoDishesFeedback
  :super ros::object
  :slots (_pecent_complete ))

(defmethod tutorial_action::DoDishesFeedback
  (:init
   (&key
    ((:pecent_complete __pecent_complete) 0)
    )
   (send-super :init)
   (setq _pecent_complete (round __pecent_complete))
   self)
  (:pecent_complete
   (&optional __pecent_complete)
   (if __pecent_complete (setq _pecent_complete __pecent_complete)) _pecent_complete)
  (:serialization-length
   ()
   (+
    ;; uint32 _pecent_complete
    4
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; uint32 _pecent_complete
       (write-long _pecent_complete s)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; uint32 _pecent_complete
     (setq _pecent_complete (sys::peek buf ptr- :integer)) (incf ptr- 4)
   ;;
   self)
  )

(setf (get tutorial_action::DoDishesFeedback :md5sum-) "dfd0cf17f480a4264705c30ba0914c92")
(setf (get tutorial_action::DoDishesFeedback :datatype-) "tutorial_action/DoDishesFeedback")
(setf (get tutorial_action::DoDishesFeedback :definition-)
      "# ====== DO NOT MODIFY! AUTOGENERATED FROM AN ACTION DEFINITION ======
# feedback
uint32 pecent_complete

")



(provide :tutorial_action/DoDishesFeedback "dfd0cf17f480a4264705c30ba0914c92")


