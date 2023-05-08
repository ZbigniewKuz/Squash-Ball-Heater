package com.example.ballheater;
import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import org.json.*;

import java.io.OutputStreamWriter;
import java.net.HttpURLConnection;
import java.net.URL;
import javax.net.ssl.HttpsURLConnection;

import org.jetbrains.annotations.NotNull;

import java.io.IOException;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.FormBody;
import okhttp3.MediaType;
import okhttp3.OkHttpClient;
import okhttp3.Request;
import okhttp3.RequestBody;
import okhttp3.Response;

public class MainActivity extends AppCompatActivity {
    Handler handler = new Handler();
    Runnable runnable;
    int delay = 4000;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button send = (Button) findViewById(R.id.send);
        send.setOnClickListener(click-> {
            try {
                putRequestTemperature();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });
        Button send_status = (Button) findViewById(R.id.start_stop);
        send_status.setOnClickListener(click-> {
            try {
                putRequestStatus();
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        });
    }
    public void putRequestTemperature() throws IOException{
        EditText new_temp = (EditText) findViewById(R.id.new_temp);
        String new_temperature = new_temp.getText().toString();
        String json = "{\"set_temperature\":\""+new_temperature+"\"}";
        String url = "https://zbigniewk.pythonanywhere.com/machine/update/1";
        MediaType JSON = MediaType.parse("application/json; charset=utf-8");
        RequestBody body = RequestBody.create(json, JSON);

        OkHttpClient client = new OkHttpClient();

        Request request = new Request.Builder()
                .url(url)
                .put(body) //PUT
                .build();

        client.newCall(request).enqueue(new Callback() {
            @Override
            public void onFailure(@NonNull Call call, @NonNull IOException e) {

            }

            @Override
            public void onResponse(@NonNull Call call, @NonNull Response response) throws IOException {

            }
        });

    }
    public void putRequestStatus() throws IOException{
        Request request = new Request.Builder().url("http://zbigniewk.pythonanywhere.com/machine/1").build();
        OkHttpClient okHttpClient = new OkHttpClient();
        okHttpClient.newCall(request).enqueue(new Callback() {

            @Override
            public void onFailure(@NotNull Call call, @NotNull IOException e) {

            }

            @Override
            public void onResponse(@NotNull Call call, @NotNull Response response) throws IOException {
                Boolean send = true;
                String response_body = response.body().string();
                try {
                    JSONObject obj = new JSONObject(response_body);
                    String new_status = obj.getString("status");
                    if(new_status == "true"){
                        send = false ;
                    }
                    else{
                        send = true ;
                    }
                    String json = "{\"status\":"+send+"}";
                    String url = "https://zbigniewk.pythonanywhere.com/machine/update_status/1";
                    MediaType JSON = MediaType.parse("application/json; charset=utf-8");
                    RequestBody body = RequestBody.create(json, JSON);

                    OkHttpClient client2 = new OkHttpClient();

                    Request request2 = new Request.Builder()
                            .url(url)
                            .put(body) //PUT
                            .build();

                    client2.newCall(request2).enqueue(new Callback() {
                        @Override
                        public void onFailure(@NonNull Call call, @NonNull IOException e) {

                        }

                        @Override
                        public void onResponse(@NonNull Call call, @NonNull Response response) throws IOException {

                        }
                    });

                }
                catch (JSONException e){

                }
            }
        });


    }
    protected void onResume() {
        handler.postDelayed(runnable = new Runnable() {
            public void run() {
                handler.postDelayed(runnable, delay);
                Request request = new Request.Builder().url("http://zbigniewk.pythonanywhere.com/action/last").build();
                OkHttpClient okHttpClient = new OkHttpClient();
                TextView present_temperature = (TextView) findViewById(R.id.present_temp);
                okHttpClient.newCall(request).enqueue(new Callback() {

                    @Override
                    public void onFailure(@NotNull Call call, @NotNull IOException e) {
                        present_temperature.setText("00");

                    }

                    @Override
                    public void onResponse(@NotNull Call call, @NotNull Response response) throws IOException {
                        String response_body = response.body().string();
                        try {
                            JSONObject obj = new JSONObject(response_body);
                            String temperature = obj.getString("action_temperature");
                            present_temperature.setText(temperature);
                        }
                        catch (JSONException e){

                        }
                    }
                });
            }
        },delay);
        super.onResume();
    }
}