
(cl:in-package :asdf)

(defsystem "learning_veddha-msg"
  :depends-on (:roslisp-msg-protocol :roslisp-utils :std_msgs-msg
)
  :components ((:file "_package")
    (:file "GpsInfo" :depends-on ("_package_GpsInfo"))
    (:file "_package_GpsInfo" :depends-on ("_package"))
  ))