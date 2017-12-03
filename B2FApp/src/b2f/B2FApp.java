package b2f;

import b2f.controllers.MainController;
import b2f.lib.B2FLib;
import javafx.application.Application;
import javafx.fxml.FXMLLoader;
import javafx.scene.Parent;
import javafx.scene.Scene;
import javafx.scene.layout.Pane;
import javafx.scene.paint.Color;
import javafx.stage.Stage;

public class B2FApp extends Application {

    @Override
    public void start(Stage primaryStage) throws Exception{



        FXMLLoader loader = new FXMLLoader(getClass().getResource("views/MainView.fxml"));

        Pane pane = (Pane) loader.load();


        primaryStage.setTitle("Hello World");
        primaryStage.setScene(new Scene(pane, 1280, 720));
        primaryStage.show();

        MainController controller = loader.<MainController>getController();
        controller.render();


    }


    public static void main(String[] args) {

       launch(args);
    }
}
