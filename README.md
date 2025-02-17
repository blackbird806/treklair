This project was originaly purposed to be a platformer but it ended up as 2d physics sim (made by @ironax).

Commands :
    Arrows keys to control the Gyrobot
    C/V/B to spawn bodies (Circle/Square/Rect)
    Z to toggle spring spawning between bodies
    F3 to clear spawned bodies
    F2 pause/play the simultation, F9 to step update

My goal of this project was to simulate an electric unicycle, mine died IRL so i made a virtual one to compensate my loss 🗿

This include full 2d rigidbody simulation, spring joints and my goal :
a cute gyrobot constitued of 1 hinge joint, a motor and a controllable spring joint

I tried to simulate physics with my own algorithms, i had previous experience with implementing SAT & GJK but this time i decided to not look up at sources and find a way myself to emulate the real world (it was hard and time expensive).

This is written in c++, windowing and line drawing is done with SDL
We experimented with c++20 Modules Feature.
