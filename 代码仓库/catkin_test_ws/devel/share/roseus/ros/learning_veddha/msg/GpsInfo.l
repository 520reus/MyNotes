;; Auto-generated. Do not edit!


(when (boundp 'learning_veddha::GpsInfo)
  (if (not (find-package "LEARNING_VEDDHA"))
    (make-package "LEARNING_VEDDHA"))
  (shadow 'GpsInfo (find-package "LEARNING_VEDDHA")))
(unless (find-package "LEARNING_VEDDHA::GPSINFO")
  (make-package "LEARNING_VEDDHA::GPSINFO"))

(in-package "ROS")
;;//! \htmlinclude GpsInfo.msg.html
(if (not (find-package "STD_MSGS"))
  (ros::roseus-add-msgs "std_msgs"))


(defclass learning_veddha::GpsInfo
  :super ros::object
  :slots (_header _status _Vx _Vy _Vz _Vv _Heading _Pitch _Roll ))

(defmethod learning_veddha::GpsInfo
  (:init
   (&key
    ((:header __header) (instance std_msgs::Header :init))
    ((:status __status) 0)
    ((:Vx __Vx) 0.0)
    ((:Vy __Vy) 0.0)
    ((:Vz __Vz) 0.0)
    ((:Vv __Vv) 0.0)
    ((:Heading __Heading) 0.0)
    ((:Pitch __Pitch) 0.0)
    ((:Roll __Roll) 0.0)
    )
   (send-super :init)
   (setq _header __header)
   (setq _status (round __status))
   (setq _Vx (float __Vx))
   (setq _Vy (float __Vy))
   (setq _Vz (float __Vz))
   (setq _Vv (float __Vv))
   (setq _Heading (float __Heading))
   (setq _Pitch (float __Pitch))
   (setq _Roll (float __Roll))
   self)
  (:header
   (&rest __header)
   (if (keywordp (car __header))
       (send* _header __header)
     (progn
       (if __header (setq _header (car __header)))
       _header)))
  (:status
   (&optional __status)
   (if __status (setq _status __status)) _status)
  (:Vx
   (&optional __Vx)
   (if __Vx (setq _Vx __Vx)) _Vx)
  (:Vy
   (&optional __Vy)
   (if __Vy (setq _Vy __Vy)) _Vy)
  (:Vz
   (&optional __Vz)
   (if __Vz (setq _Vz __Vz)) _Vz)
  (:Vv
   (&optional __Vv)
   (if __Vv (setq _Vv __Vv)) _Vv)
  (:Heading
   (&optional __Heading)
   (if __Heading (setq _Heading __Heading)) _Heading)
  (:Pitch
   (&optional __Pitch)
   (if __Pitch (setq _Pitch __Pitch)) _Pitch)
  (:Roll
   (&optional __Roll)
   (if __Roll (setq _Roll __Roll)) _Roll)
  (:serialization-length
   ()
   (+
    ;; std_msgs/Header _header
    (send _header :serialization-length)
    ;; uint8 _status
    1
    ;; float64 _Vx
    8
    ;; float64 _Vy
    8
    ;; float64 _Vz
    8
    ;; float64 _Vv
    8
    ;; float64 _Heading
    8
    ;; float64 _Pitch
    8
    ;; float64 _Roll
    8
    ))
  (:serialize
   (&optional strm)
   (let ((s (if strm strm
              (make-string-output-stream (send self :serialization-length)))))
     ;; std_msgs/Header _header
       (send _header :serialize s)
     ;; uint8 _status
       (write-byte _status s)
     ;; float64 _Vx
       (sys::poke _Vx (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _Vy
       (sys::poke _Vy (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _Vz
       (sys::poke _Vz (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _Vv
       (sys::poke _Vv (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _Heading
       (sys::poke _Heading (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _Pitch
       (sys::poke _Pitch (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;; float64 _Roll
       (sys::poke _Roll (send s :buffer) (send s :count) :double) (incf (stream-count s) 8)
     ;;
     (if (null strm) (get-output-stream-string s))))
  (:deserialize
   (buf &optional (ptr- 0))
   ;; std_msgs/Header _header
     (send _header :deserialize buf ptr-) (incf ptr- (send _header :serialization-length))
   ;; uint8 _status
     (setq _status (sys::peek buf ptr- :char)) (incf ptr- 1)
   ;; float64 _Vx
     (setq _Vx (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _Vy
     (setq _Vy (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _Vz
     (setq _Vz (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _Vv
     (setq _Vv (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _Heading
     (setq _Heading (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _Pitch
     (setq _Pitch (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;; float64 _Roll
     (setq _Roll (sys::peek buf ptr- :double)) (incf ptr- 8)
   ;;
   self)
  )

(setf (get learning_veddha::GpsInfo :md5sum-) "81490e56783fc6d70c2658a0fe52a3c4")
(setf (get learning_veddha::GpsInfo :datatype-) "learning_veddha/GpsInfo")
(setf (get learning_veddha::GpsInfo :definition-)
      "
Header header

uint8 status

float64 Vx
float64 Vy
float64 Vz
float64 Vv
float64 Heading
float64 Pitch
float64 Roll
================================================================================
MSG: std_msgs/Header
# Standard metadata for higher-level stamped data types.
# This is generally used to communicate timestamped data 
# in a particular coordinate frame.
# 
# sequence ID: consecutively increasing ID 
uint32 seq
#Two-integer timestamp that is expressed as:
# * stamp.sec: seconds (stamp_secs) since epoch (in Python the variable is called 'secs')
# * stamp.nsec: nanoseconds since stamp_secs (in Python the variable is called 'nsecs')
# time-handling sugar is provided by the client library
time stamp
#Frame this data is associated with
string frame_id

")



(provide :learning_veddha/GpsInfo "81490e56783fc6d70c2658a0fe52a3c4")


