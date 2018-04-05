package com.example.nikita.myapplication;

import android.app.Service;
import android.content.Intent;
import android.location.Location;
import android.os.Binder;

import android.os.IBinder;

import com.google.android.gms.location.LocationSettingsRequest;
import com.google.android.gms.location.LocationListener;
import com.google.android.gms.location.LocationSettingsResponse;
import com.google.android.gms.location.LocationRequest;
import com.google.android.gms.location.LocationCallback;
import com.google.android.gms.location.LocationResult;
import com.google.android.gms.location.LocationServices;
import com.google.android.gms.location.FusedLocationProviderClient;
import android.support.annotation.Nullable;
import com.google.android.gms.tasks.Task;

import java.text.DecimalFormat;
import java.util.concurrent.TimeUnit;


public class Check_location extends Service implements
        LocationListener {

    private static final long INTERVAL = 500;
    private static final long FASTEST_INTERVAL = 200;

    LocationRequest location_request_low;
    LocationRequest location_request_high;
    LocationSettingsRequest locationSettingsRequest;
    Location cur_location, l_start, l_end;
    private FusedLocationProviderClient fused_location_client;

    static double distance = 0;
    double speed = 0;

    private final IBinder l_binder = new LocalBinder();//

    public class LocalBinder extends Binder {
        public Check_location getService() {
            return Check_location.this;
        }
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        return l_binder;
    }

    public void contructService() {
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

        try {
            fused_location_client.requestLocationUpdates(location_request_high, new LocationCallback() {
                        @Override
                        public void onLocationResult(LocationResult locationResult) {
                            // do work here
                            onLocationChanged(locationResult.getLastLocation());
                        }
                    },
                    null);
        } catch (SecurityException e) {}
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        return super.onStartCommand(intent, flags, startId);
    }


    @Override
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
        speed = location.getSpeed() * 3.6;
    }

    //The live feed of Distance and Speed are being set in the method below .
    private void updateUI() {
        if (MainActivity.p == 0) {
            distance = distance + (l_start.distanceTo(l_end) / 1000.00);
            MainActivity.end_time = System.currentTimeMillis();
            long diff = MainActivity.end_time - MainActivity.start_time;
            diff = TimeUnit.MILLISECONDS.toMinutes(diff);
            MainActivity.time.setText("Total Time: " + diff + " min");
            if (speed > 0.0)
                MainActivity.speed.setText("Current speed: " + new DecimalFormat("#.##").format(speed) + " km/h");
            else
                MainActivity.speed.setText(".......");

            MainActivity.dist.setText(new DecimalFormat("#.###").format(distance) + "from starting.");

            l_start = l_end;

        }

    }


    @Override
    public boolean onUnbind(Intent intent) {
        //stopLocationUpdates();

        l_start = null;
        l_end = null;
        distance = 0;
        return super.onUnbind(intent);
    }
}
