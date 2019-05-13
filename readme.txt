non-library link type

        util.h
           |
        mutil.h		message.h
           |		    |
           +----------------+
                   |
                vector.h
                   |
                matrix.h

        sort.h
        list.h


library link type
                  vector.h
                     |
            +-----------------+
            |		      |
        viewport.h	  geometry.h
      (viewport.cpp)	(geometry.cpp)
            |		      |
            +-----------------+
                     |
                   graph.h
                 (graph.cpp)
                     |
                  canvas.h
                (canvas.cpp)
                     |
                   hook.h
                 (hook.cpp)
