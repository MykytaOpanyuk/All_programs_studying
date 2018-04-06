package com.example.nikita.myapplication;

import android.Manifest;
import android.app.AlertDialog;
import android.app.ProgressDialog;
import android.content.ComponentName;
import android.content.DialogInterface;
import android.content.Intent;
import android.content.ServiceConnection;
import android.location.Location;
import android.location.LocationManager;
import android.os.Bundle;
import android.os.IBinder;
import android.app.Activity;
import android.content.Context;
import android.support.v4.app.ActivityCompat;
import android.widget.TextView;
import android.app.PendingIntent;

import com.google.android.gms.location.FusedLocationProviderClient;
import com.google.android.gms.location.LocationListener;
import com.google.android.gms.location.LocationRequest;
import com.google.android.gms.location.LocationServices;
import com.google.android.gms.location.LocationSettingsRequest;
import com.google.android.gms.location.LocationSettingsResponse;
import com.google.android.gms.tasks.Task;
import com.google.android.gms.tasks.OnFailureListener;
import android.content.pm.PackageManager;

import android.support.annotation.NonNull;
import java.text.DecimalFormat;
import java.util.concurrent.TimeUnit;


public class MainActivity extends Activity {

    //---Finding speed---//
    private static final long INTERVAL = 500;
    private static final long FASTEST_INTERVAL = 200;

    public LocationRequest location_request_low;
    public LocationRequest location_request_high;
    public Location cur_location, l_start, l_end;
    public FusedLocationProviderClient fused_location_client;

    public static double distance = 0;
    public double double_speed = 0;
    //---Finding speed---//

    //---Initialize---//
    static Check_location my_service;
    static boolean status;
    static LocationManager location_manager;
    static TextView dist, time, speed;
    static long start_time, end_time;
    static ProgressDialog locate;
    static int p = 0;
    Intent i;
    PendingIntent pend_int;

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
        l_start = null;
        l_end = null;
        distance = 0;
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

            location_request_high = LocationRequest.create();
            location_request_high.setInterval(INTERVAL);
            location_request_high.setFastestInterval(FASTEST_INTERVAL);
            location_request_high.setPriority(LocationRequest.PRIORITY_HIGH_ACCURACY);

            location_request_low = LocationRequest.create();
            location_request_low.setInterval(INTERVAL * 2);
            location_request_low.setFastestInterval(FASTEST_INTERVAL * 2);
            location_request_low.setPriority(LocationRequest.PRIORITY_BALANCED_POWER_ACCURACY);

            LocationSettingsRequest.Builder builder = new LocationSettingsRequest.Builder()
                    .addLocationRequest(location_request_high)
                    .addLocationRequest(location_request_low);


            Task<LocationSettingsResponse> result =
                    LocationServices.getSettingsClient(this).checkLocationSettings(builder.build()); // MISTAKE

            fused_location_client = LocationServices.getFusedLocationProviderClient(this);

            if (ActivityCompat.checkSelfPermission(this, android.Manifest.permission.ACCESS_FINE_LOCATION)
                    == PackageManager.PERMISSION_GRANTED) {
                fused_location_client.requestLocationUpdates(LocationRequest.create(), pend_int)
                        .addOnFailureListener(new OnFailureListener() {
                            @Override
                            public void onFailure(@NonNull Exception e) {
                                onLocationChanged(cur_location);
                            }
                        });
                locate = new ProgressDialog(MainActivity.this);
                locate.setIndeterminate(true);
                locate.setCancelable(false);
                // add changing of speed
            }


        }
    }

    public void onLocationChanged(Location location) {
        MainActivity.locate.dismiss();
        cur_location = location;
        if (l_start == null) {
            l_start = cur_location;
            l_end = cur_location;
        } else
            l_end = cur_location;

        //Calling the method below updates the  live values of distance and speed to the TextViews.
        updateUI();
        //calculating the speed with getSpeed method it returns speed in m/s so we are converting it into kmph
        double_speed = location.getSpeed() * 3.6;
    }

    //The live feed of Distance and Speed are being set in the method below .
    private void updateUI() {
        if (MainActivity.p == 0) {
            distance = distance + (l_start.distanceTo(l_end) / 1000.00);
            MainActivity.end_time = System.currentTimeMillis();
            long diff = MainActivity.end_time - MainActivity.start_time;
            diff = TimeUnit.MILLISECONDS.toMinutes(diff);
            MainActivity.time.setText("Total Time: " + diff + " min");
            if (double_speed > 0.0)
                MainActivity.speed.setText("Current speed: " + new DecimalFormat("#.##").format(speed) + " km/h");
            else
                MainActivity.speed.setText(".......");

            MainActivity.dist.setText(new DecimalFormat("#.###").format(distance) + "from starting.");

            l_start = l_end;

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