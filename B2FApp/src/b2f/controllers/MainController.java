package b2f.controllers;

import b2f.Pixel;
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

        int xSize = 512;
        int ySize = 512;

        Pixel[] image = B2FLib.render(xSize, ySize, 512);
        canvas.getGraphicsContext2D().fillRect(0,0,1000, 1000);

        try {
            for (Pixel p : image) {
                pixelWriter.setColor(xSize - p.getX(), ySize - p.getY(), Color.rgb(p.getR(), p.getG(), p.getB()));
            }
        } catch (Exception e)
        {
            e.printStackTrace();
        }


    }


}
