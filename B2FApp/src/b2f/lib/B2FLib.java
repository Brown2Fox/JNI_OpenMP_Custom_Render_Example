package b2f.lib;

import b2f.Point;

public class B2FLib {

    public static native void calc(int a);
    public static native Point[] render(int xSize, int ySize);


    static {
        System.load("C:\\Users\\Antti\\Workflow\\BBB\\outlib\\b2flib.dll");
//        System.load("C:\\Users\\Antti\\Workflow\\BBB\\test.dll");
    }
}