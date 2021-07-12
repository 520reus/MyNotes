
(cl:in-package :asdf)

(defsystem "costmap_converter-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :geometry_msgs-msg
               :std_msgs-msg
)
  :components ((:file "_package")
    (:file "ObstacleArrayMsg" :depends-on ("_package_ObstacleArrayMsg"))
    (:file "_package_ObstacleArrayMsg" :depends-on ("_package"))
    (:file "ObstacleMsg" :depends-on ("_package_ObstacleMsg"))
    (:file "_package_ObstacleMsg" :depends-on ("_package"))
  ))