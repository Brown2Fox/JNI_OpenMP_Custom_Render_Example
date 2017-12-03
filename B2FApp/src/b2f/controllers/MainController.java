package b2f.controllers;

import b2f.Point;
import b2f.lib.B2FLib;
import javafx.fxml.FXML;
import javafx.scene.canvas.Canvas;
import javafx.scene.image.PixelWriter;
import javafx.scene.paint.Color;

public class MainController {

    @FXML
    Canvas canvas;

    private PixelWriter pixelWriter;


    @FXML
    public void initialize() {
        pixelWriter = canvas.getGraphicsContext2D().getPixelWriter();
    }

    public void render() {

        int xSize = 12;
        int ySize = 100;

        Point[] points = B2FLib.render(xSize, ySize);

        int x = 0;
        int y = 0;
        for (Point p: points) {
            pixelWriter.setColor(x, y, Color.rgb(p.getR(), p.getG(), p.getB()));
            if (++x == xSize) { y++; x = 0; }
        }
    }


}
