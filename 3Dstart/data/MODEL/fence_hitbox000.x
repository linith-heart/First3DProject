xof 0303txt 0032

Frame Root {
  FrameTransformMatrix {
     1.000000, 0.000000, 0.000000, 0.000000,
     0.000000,-0.000000, 1.000000, 0.000000,
     0.000000, 1.000000, 0.000000, 0.000000,
     0.000000, 0.000000, 0.000000, 1.000000;;
  }
  Frame Cube {
    FrameTransformMatrix {
      20.000000, 0.000000, 0.000000, 0.000000,
       0.000000,20.000000, 0.000000, 0.000000,
       0.000000, 0.000000,20.000000, 0.000000,
       0.000000, 0.000000, 0.000000, 1.000000;;
    }
    Mesh { // Cube mesh
      8;
      -0.200000;-7.500000;-1.000000;,
      -0.200000;-7.500000; 4.250000;,
       0.200000;-7.500000;-1.000000;,
       0.200000;-7.500000; 4.250000;,
      -0.200000; 7.500000;-1.000000;,
      -0.200000; 7.500000; 4.250000;,
       0.200000; 7.500000;-1.000000;,
       0.200000; 7.500000; 4.250000;;
      10;
      3;3,7,6;,
      3;2,0,3;,
      3;4,5,1;,
      3;0,1,3;,
      3;1,5,7;,
      3;4,6,5;,
      3;0,4,1;,
      3;3,1,7;,
      3;2,3,6;,
      3;6,7,5;;
      MeshNormals { // Cube normals
        10;
         1.000000;-0.000000; 0.000000;,
         0.000000;-1.000000; 0.000000;,
        -1.000000; 0.000000; 0.000000;,
         0.000000;-1.000000; 0.000000;,
         0.000000; 0.000000; 1.000000;,
         0.000000; 1.000000;-0.000000;,
        -1.000000; 0.000000; 0.000000;,
         0.000000;-0.000000; 1.000000;,
         1.000000;-0.000000; 0.000000;,
         0.000000; 1.000000;-0.000000;;
        10;
        3;0,0,0;,
        3;1,1,1;,
        3;2,2,2;,
        3;3,3,3;,
        3;4,4,4;,
        3;5,5,5;,
        3;6,6,6;,
        3;7,7,7;,
        3;8,8,8;,
        3;9,9,9;;
      } // End of Cube normals
    } // End of Cube mesh
  } // End of Cube
} // End of Root
