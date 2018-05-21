docker run -u $(id -u):1000 -v $(pwd):/home/develboard  -it develboard-qt-gcc /bin/bash -c "source /opt/develboard/1.8.1/environment-setup-cortexa5hf-vfp-neon-develer-linux-gnueabi && qmake && make"
