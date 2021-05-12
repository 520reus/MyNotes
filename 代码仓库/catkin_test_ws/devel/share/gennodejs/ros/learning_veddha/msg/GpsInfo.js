// Auto-generated. Do not edit!

// (in-package learning_veddha.msg)


"use strict";

const _serializer = _ros_msg_utils.Serialize;
const _arraySerializer = _serializer.Array;
const _deserializer = _ros_msg_utils.Deserialize;
const _arrayDeserializer = _deserializer.Array;
const _finder = _ros_msg_utils.Find;
const _getByteLength = _ros_msg_utils.getByteLength;
let std_msgs = _finder('std_msgs');

//-----------------------------------------------------------

class GpsInfo {
  constructor(initObj={}) {
    if (initObj === null) {
      // initObj === null is a special case for deserialization where we don't initialize fields
      this.header = null;
      this.status = null;
      this.Vx = null;
      this.Vy = null;
      this.Vz = null;
      this.Vv = null;
      this.Heading = null;
      this.Pitch = null;
      this.Roll = null;
    }
    else {
      if (initObj.hasOwnProperty('header')) {
        this.header = initObj.header
      }
      else {
        this.header = new std_msgs.msg.Header();
      }
      if (initObj.hasOwnProperty('status')) {
        this.status = initObj.status
      }
      else {
        this.status = 0;
      }
      if (initObj.hasOwnProperty('Vx')) {
        this.Vx = initObj.Vx
      }
      else {
        this.Vx = 0.0;
      }
      if (initObj.hasOwnProperty('Vy')) {
        this.Vy = initObj.Vy
      }
      else {
        this.Vy = 0.0;
      }
      if (initObj.hasOwnProperty('Vz')) {
        this.Vz = initObj.Vz
      }
      else {
        this.Vz = 0.0;
      }
      if (initObj.hasOwnProperty('Vv')) {
        this.Vv = initObj.Vv
      }
      else {
        this.Vv = 0.0;
      }
      if (initObj.hasOwnProperty('Heading')) {
        this.Heading = initObj.Heading
      }
      else {
        this.Heading = 0.0;
      }
      if (initObj.hasOwnProperty('Pitch')) {
        this.Pitch = initObj.Pitch
      }
      else {
        this.Pitch = 0.0;
      }
      if (initObj.hasOwnProperty('Roll')) {
        this.Roll = initObj.Roll
      }
      else {
        this.Roll = 0.0;
      }
    }
  }

  static serialize(obj, buffer, bufferOffset) {
    // Serializes a message object of type GpsInfo
    // Serialize message field [header]
    bufferOffset = std_msgs.msg.Header.serialize(obj.header, buffer, bufferOffset);
    // Serialize message field [status]
    bufferOffset = _serializer.uint8(obj.status, buffer, bufferOffset);
    // Serialize message field [Vx]
    bufferOffset = _serializer.float64(obj.Vx, buffer, bufferOffset);
    // Serialize message field [Vy]
    bufferOffset = _serializer.float64(obj.Vy, buffer, bufferOffset);
    // Serialize message field [Vz]
    bufferOffset = _serializer.float64(obj.Vz, buffer, bufferOffset);
    // Serialize message field [Vv]
    bufferOffset = _serializer.float64(obj.Vv, buffer, bufferOffset);
    // Serialize message field [Heading]
    bufferOffset = _serializer.float64(obj.Heading, buffer, bufferOffset);
    // Serialize message field [Pitch]
    bufferOffset = _serializer.float64(obj.Pitch, buffer, bufferOffset);
    // Serialize message field [Roll]
    bufferOffset = _serializer.float64(obj.Roll, buffer, bufferOffset);
    return bufferOffset;
  }

  static deserialize(buffer, bufferOffset=[0]) {
    //deserializes a message object of type GpsInfo
    let len;
    let data = new GpsInfo(null);
    // Deserialize message field [header]
    data.header = std_msgs.msg.Header.deserialize(buffer, bufferOffset);
    // Deserialize message field [status]
    data.status = _deserializer.uint8(buffer, bufferOffset);
    // Deserialize message field [Vx]
    data.Vx = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [Vy]
    data.Vy = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [Vz]
    data.Vz = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [Vv]
    data.Vv = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [Heading]
    data.Heading = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [Pitch]
    data.Pitch = _deserializer.float64(buffer, bufferOffset);
    // Deserialize message field [Roll]
    data.Roll = _deserializer.float64(buffer, bufferOffset);
    return data;
  }

  static getMessageSize(object) {
    let length = 0;
    length += std_msgs.msg.Header.getMessageSize(object.header);
    return length + 57;
  }

  static datatype() {
    // Returns string type for a message object
    return 'learning_veddha/GpsInfo';
  }

  static md5sum() {
    //Returns md5sum for a message object
    return '81490e56783fc6d70c2658a0fe52a3c4';
  }

  static messageDefinition() {
    // Returns full string definition for message
    return `
    
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
    
    `;
  }

  static Resolve(msg) {
    // deep-construct a valid message object instance of whatever was passed in
    if (typeof msg !== 'object' || msg === null) {
      msg = {};
    }
    const resolved = new GpsInfo(null);
    if (msg.header !== undefined) {
      resolved.header = std_msgs.msg.Header.Resolve(msg.header)
    }
    else {
      resolved.header = new std_msgs.msg.Header()
    }

    if (msg.status !== undefined) {
      resolved.status = msg.status;
    }
    else {
      resolved.status = 0
    }

    if (msg.Vx !== undefined) {
      resolved.Vx = msg.Vx;
    }
    else {
      resolved.Vx = 0.0
    }

    if (msg.Vy !== undefined) {
      resolved.Vy = msg.Vy;
    }
    else {
      resolved.Vy = 0.0
    }

    if (msg.Vz !== undefined) {
      resolved.Vz = msg.Vz;
    }
    else {
      resolved.Vz = 0.0
    }

    if (msg.Vv !== undefined) {
      resolved.Vv = msg.Vv;
    }
    else {
      resolved.Vv = 0.0
    }

    if (msg.Heading !== undefined) {
      resolved.Heading = msg.Heading;
    }
    else {
      resolved.Heading = 0.0
    }

    if (msg.Pitch !== undefined) {
      resolved.Pitch = msg.Pitch;
    }
    else {
      resolved.Pitch = 0.0
    }

    if (msg.Roll !== undefined) {
      resolved.Roll = msg.Roll;
    }
    else {
      resolved.Roll = 0.0
    }

    return resolved;
    }
};

module.exports = GpsInfo;
