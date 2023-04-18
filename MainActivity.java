package com.example.ballheater;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Context;
import android.os.Bundle;
import android.os.Handler;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import org.jetbrains.annotations.NotNull;

import java.io.IOException;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

import okhttp3.Call;
import okhttp3.Callback;
import okhttp3.OkHttpClient;
import okhttp3.Request;
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
        EditText new_temp = (EditText) findViewById(R.id.new_temp);

        Request request = new Request.Builder().url("http://zbigniewk.pythonanywhere.com/action/last").build();
        OkHttpClient okHttpClient = new OkHttpClient();

        send.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                String text = new_temp.getText().toString();
                //do dokończenia
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
                        present_temperature.setText(response_body.substring(36, 40));
                    }
                    });
            }


        },delay);
        super.onResume();
    }
}