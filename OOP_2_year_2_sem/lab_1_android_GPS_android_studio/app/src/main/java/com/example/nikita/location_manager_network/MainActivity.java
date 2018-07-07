package com.example.nikita.location_manager_network;

import android.Manifest;
import android.content.pm.PackageManager;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.app.AlertDialog;
import android.content.DialogInterface;

import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;

import com.google.android.gms.location.LocationRequest;
import com.google.android.gms.location.LocationServices;
import com.google.android.gms.location.FusedLocationProviderClient;
import com.google.android.gms.location.LocationCallback;
import com.google.android.gms.location.LocationResult;

import java.text.DecimalFormat;
import android.content.Intent;
import android.content.Context;
import android.widget.TextView;
import android.provider.Settings;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    private static final int two_second = 2000;

    public FusedLocationProviderClient fused_location_client;
    LocationRequest location_request_high;
    LocationCallback location_callback;
    Context mContext;
    //static long start_time, end_time;
    public Location l_start, l_end;
    public Location location_cur, location_old;
    LocationManager location_manager;

    LocationListener my_listener = new LocationListener() {
        public void onLocationChanged(Location location) {
            if (location_cur == null)
                location_cur = location;
                location_old = location;

            long timeDelta = location_cur.getTime() - location.getTime();
            boolean isSignificantlyNewer = timeDelta > two_second;
            boolean isNewer = timeDelta > 0;

            if (isSignificantlyNewer)
                location_old = location;

            int accuracyDelta = (int) (location.getAccuracy() - location_cur.getAccuracy());
            boolean isLessAccurate = accuracyDelta > 0;
            boolean isMoreAccurate = accuracyDelta < 0;
            boolean isSignificantlyLessAccurate = accuracyDelta > 200;

            if (isMoreAccurate)
                location_old = location;
            else if (isNewer && !isLessAccurate)
                location_old = location;
            else if ((isNewer) && !(isSignificantlyLessAccurate))
                location_old = location;

            double speed = location.getSpeed() * 3.6;

            String speed_2 = "Speed (LocationManager): " + new DecimalFormat("#.##").format(speed) + "km/h";

            TextView speed_text = findViewById(R.id.speed_2);
            speed_text.setText(speed_2);
        }

        public void onStatusChanged(String provider, int status, Bundle extras) {}

        public void onProviderEnabled(String provider) {}

        public void onProviderDisabled(String provider) {}
    };

    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
        switch (requestCode) {
            case 1: {
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {}
                else
                    return;
            }
            // other 'case' lines to check for other
            // permissions this app might request
        }
    }

    public void checkLocationPermission()
    {
        String permission_1 = "android.permission.ACCESS_FINE_LOCATION";
        String permission_2 = "android.permission.ACCESS_COARSE_LOCATION";

        int res_1 = this.checkCallingOrSelfPermission(permission_1);
        int res_2 = this.checkCallingOrSelfPermission(permission_2);

        if ((res_1 == PackageManager.PERMISSION_GRANTED) && (res_2 == PackageManager.PERMISSION_GRANTED)) {
            fused_location_client.requestLocationUpdates(location_request_high, location_callback, null);
            location_manager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 1000, 10, my_listener);
        }

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        mContext = this;
        location_manager = (LocationManager) mContext.getSystemService(Context.LOCATION_SERVICE);

        location_request_high = LocationRequest.create();
        location_request_high.setInterval(1000);
        location_request_high.setFastestInterval(500);
        location_request_high.setPriority(LocationRequest.PRIORITY_HIGH_ACCURACY);

        fused_location_client = LocationServices.getFusedLocationProviderClient(this);

        location_callback = new LocationCallback() {
            @Override
            public void onLocationResult(LocationResult locationResult) {
                if (locationResult == null) {
                    return;
                }
                for (Location location : locationResult.getLocations()) {
                    makeUseOfNewLocation(location);
                }
            }
        };
        ActivityCompat.requestPermissions(this,new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, 1);
        ActivityCompat.requestPermissions(this,new String[]{Manifest.permission.ACCESS_COARSE_LOCATION}, 1);

        checkLocationPermission();

        isLocationEnabled();
        isLocationEnabled_manager();

    }

    public void makeUseOfNewLocation(Location location) {

        if (l_start == null)
            l_start = location;
            l_end = location;

        long timeDelta = l_start.getTime() - location.getTime();
        boolean isSignificantlyNewer = timeDelta > two_second;
        boolean isNewer = timeDelta > 0;

        if (isSignificantlyNewer)
            l_end = location;

        int accuracyDelta = (int) (location.getAccuracy() - l_start.getAccuracy());
        boolean isLessAccurate = accuracyDelta > 0;
        boolean isMoreAccurate = accuracyDelta < 0;
        boolean isSignificantlyLessAccurate = accuracyDelta > 200;

        if (isMoreAccurate)
            l_end = location;
        else if (isNewer && !isLessAccurate)
            l_end = location;
        else if ((isNewer) && !(isSignificantlyLessAccurate))
            l_end = location;

        double latitude = location.getLatitude();
        double longitude = location.getLongitude();
        double speed = location.getSpeed() * 3.6;

        String msg = "New Latitude: " + latitude + "New Longitude: " + longitude;
        String speed_1 = "Speed (FusedClient): " + new DecimalFormat("#.##").format(speed) + "km/h";

        TextView speed_f_text = findViewById(R.id.speed_1);
        speed_f_text.setText(speed_1);
        TextView time_text = findViewById(R.id.activity_location);
        time_text.setText(msg);

    }

    protected void onResume() {
        super.onResume();
        isLocationEnabled();
    }

    private void isLocationEnabled() {

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

    private void isLocationEnabled_manager() {

        if (!location_manager.isProviderEnabled(LocationManager.GPS_PROVIDER)) {
            AlertDialog.Builder alertDialog=new AlertDialog.Builder(mContext);
            alertDialog.setTitle("Enable Location");
            alertDialog.setMessage("Your locations setting is not enabled. Please enabled it in settings menu.");
            alertDialog.setPositiveButton("Location Settings", new DialogInterface.OnClickListener(){
                public void onClick(DialogInterface dialog, int which) {
                    Intent intent = new Intent(Settings.ACTION_LOCATION_SOURCE_SETTINGS);
                    startActivity(intent);
                }
            });
            alertDialog.setNegativeButton("Cancel", new DialogInterface.OnClickListener(){
                public void onClick(DialogInterface dialog, int which){
                    dialog.cancel();
                }
            });
            AlertDialog alert=alertDialog.create();
            alert.show();
        } else {
            AlertDialog.Builder alertDialog=new AlertDialog.Builder(mContext);
            alertDialog.setTitle("Confirm Location");
            alertDialog.setMessage("Your Location is enabled, please enjoy");
            alertDialog.setNegativeButton("Back to interface", new DialogInterface.OnClickListener(){
                public void onClick(DialogInterface dialog, int which){
                    dialog.cancel();
                }
            });
            AlertDialog alert = alertDialog.create();
            alert.show();
        }
    }
}