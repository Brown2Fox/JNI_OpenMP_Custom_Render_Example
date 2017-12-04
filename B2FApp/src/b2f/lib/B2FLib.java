package b2f.lib;

import b2f.Pixel;

public class B2FLib {

    public static native void calc(int a);
    public static native Pixel[] render(int width, int height, int depth);


    static {
        System.load("C:\\Users\\Antti\\Workflow\\BBB\\outlib\\b2flib.dll");
//        System.load("C:\\Users\\Antti\\Workflow\\BBB\\test.dll");
    }
}