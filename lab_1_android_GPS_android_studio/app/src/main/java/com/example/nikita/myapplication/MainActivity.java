package com.example.nikita.myapplication;

import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.ComponentName;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.ServiceConnection;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.IBinder;
import android.app.Activity;
import android.content.Context;
import android.widget.TextView;

public class MainActivity extends Activity {

    Check_location my_service;
    boolean status;
    LocationManager location_manager;
    static TextView dist, time, speed;
    static long start_time, end_time;
    static ProgressDialog locate;
    static int p = 0;
    Intent i;


    private ServiceConnection sc = new ServiceConnection() {
        @Override
        public void onServiceConnected(ComponentName name, IBinder service) {
            Check_location.LocalBinder binder = (Check_location.LocalBinder) service;
            my_service = binder.getService();
            status = true;
        }

        @Override
        public void onServiceDisconnected(ComponentName name) {
            status = false;
        }
    };

    void bindService() {
        if (status)
            return;
        status = true;
        start_time = System.currentTimeMillis();
    }

    void unbindService() {
        if (status)
            return;
        i = new Intent(getApplicationContext(), Check_location.class);
        unbindService(sc);
        status = false;
    }

    @Override
    protected void onResume() {
        super.onResume();

    }

    @Override
    protected void onStart() {
        super.onStart();
        i = new Intent(getApplicationContext(), Check_location.class);
        bindService(i, sc, Context.BIND_AUTO_CREATE);
    }


    @Override
    protected void onDestroy() {
        super.onDestroy();
        if (status)
            unbindService();
    }

    @Override
    public void onBackPressed() {
        if (status)
            super.onBackPressed();
        else
            moveTaskToBack(true);
    }


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        my_service = new Check_location();
        dist = findViewById(R.id.distancetext);
        time = findViewById(R.id.timetext);
        speed = findViewById(R.id.speedtext);

        checkGps();
        location_manager = (LocationManager) getSystemService(LOCATION_SERVICE);

        if (!location_manager.isProviderEnabled(LocationManager.GPS_PROVIDER)) {
            return;
        }

        if (!status) {
            //Here, the Location Service gets bound and the GPS Speedometer gets Active.
            bindService();

            my_service.contructService();
            locate = new ProgressDialog(MainActivity.this);
            locate.setIndeterminate(true);
            locate.setCancelable(false);
        }

    }


    //This method leads you to the alert dialog box.
    void checkGps() {
        location_manager = (LocationManager) getSystemService(LOCATION_SERVICE);

        if (!location_manager.isProviderEnabled(LocationManager.GPS_PROVIDER)) {
            showGPSDisabledAlertToUser();
        }
    }

    //This method configures the Alert Dialog box.
    private void showGPSDisabledAlertToUser() {
        AlertDialog.Builder alertDialogBuilder = new AlertDialog.Builder(this);
        alertDialogBuilder.setMessage("Enable GPS to use application")
                .setCancelable(false)
                .setPositiveButton("Enable GPS",
                        new DialogInterface.OnClickListener() {
                            public void onClick(DialogInterface dialog, int id) {
                                Intent callGPSSettingIntent = new Intent(
                                        android.provider.Settings.ACTION_LOCATION_SOURCE_SETTINGS);
                                startActivity(callGPSSettingIntent);
                            }
                        });
        alertDialogBuilder.setNegativeButton("Cancel",
                new DialogInterface.OnClickListener() {
                    public void onClick(DialogInterface dialog, int id) {
                        dialog.cancel();
                    }
                });
        AlertDialog alert = alertDialogBuilder.create();
        alert.show();
    }

}