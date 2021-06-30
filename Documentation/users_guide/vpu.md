Windows 10 IoT Video Processing Unit on i.MX Platform {#vpu}
==============

This chapter describes the Video Processing Unit on i.MX8 Quad and i.MX8 Mini.

## Features
   * HW Support of playback video clips encoded with h264 and h265 codec.
   * Media Transport Foundation API supported.

## Limitations
   * GPU operations such as resize/scale/crop etc. don't have HW support. These 
     operations are computed by CPU and may cause high CPU load and playback may be very slow.
     Thus, delivered IoTCore Media Player has disabled scaling by default. However clips having 
     different  video resolution and buffer dimensions force scale operation in VPU driver, 
     that may also cause high CPU load. Progressive clips also require crop operation. 
     This limitation will be resolved by implementation of GPU driver.
 
## How to play video

  1. Boot WIN10 on the target. 
  1. Download test clip:  
     1. https://mango.blender.org/download/
     2. http://ftp.halifax.rwth-aachen.de/blender/demo/movies/ToS/tears_of_steel_1080p.mov
  1. Copy clip on USB drive and attach or copy movie directly into target to this location:
     1. C:\Data\Users\DefaultAccount\Videos\
  1. On the target, from Start run IoTCore Media Player. 
     1. Click "browse" and select "RemovableStorage".
     2. Find your movie and select. 
     3. Video should play automatically. Otherwise click play button below. 
  
     