#!/bin/sh

if [ -n "$DESTDIR" ] ; then
    case $DESTDIR in
        /*) # ok
            ;;
        *)
            /bin/echo "DESTDIR argument must be absolute... "
            /bin/echo "otherwise python's distutils will bork things."
            exit 1
    esac
fi

echo_and_run() { echo "+ $@" ; "$@" ; }

echo_and_run cd "/home/wu/navigation_ws_1/src/navigation/base_local_planner"

# ensure that Python install destination exists
echo_and_run mkdir -p "$DESTDIR/home/wu/navigation_ws_1/install/lib/python2.7/dist-packages"

# Note that PYTHONPATH is pulled from the environment to support installing
# into one location when some dependencies were installed in another
# location, #123.
echo_and_run /usr/bin/env \
    PYTHONPATH="/home/wu/navigation_ws_1/install/lib/python2.7/dist-packages:/home/wu/navigation_ws_1/build/lib/python2.7/dist-packages:$PYTHONPATH" \
    CATKIN_BINARY_DIR="/home/wu/navigation_ws_1/build" \
    "/usr/bin/python2" \
    "/home/wu/navigation_ws_1/src/navigation/base_local_planner/setup.py" \
     \
    build --build-base "/home/wu/navigation_ws_1/build/navigation/base_local_planner" \
    install \
    --root="${DESTDIR-/}" \
    --install-layout=deb --prefix="/home/wu/navigation_ws_1/install" --install-scripts="/home/wu/navigation_ws_1/install/bin"
