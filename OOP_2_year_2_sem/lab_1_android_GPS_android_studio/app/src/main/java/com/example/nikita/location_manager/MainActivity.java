package com.example.nikita.location_manager;

import android.Manifest;
import android.content.pm.PackageManager;
import android.location.Location;
import android.support.v4.app.ActivityCompat;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.app.AlertDialog;
import android.content.DialogInterface;

import android.content.Intent;
import android.provider.Settings;
import android.content.Context;
import android.location.LocationManager;
import android.location.LocationListener;
import android.widget.TextView;
import android.widget.Toast;

public class MainActivity extends AppCompatActivity {

    LocationManager locationManager;
    Context mContext;
    double distance;
    public Location l_start, l_end;

    public void onRequestPermissionsResult(int requestCode, String permissions[], int[] grantResults) {
        switch (requestCode) {
            case 1: {
                // If request is cancelled, the result arrays are empty.
                if (grantResults.length > 0
                        && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                } else
                return;
            }
            // other 'case' lines to check for other
            // permissions this app might request
        }
    }

    public void checkLocationPermission()
    {
        String permission_1 = "android.permission.ACCESS_FINE_LOCATION";

        int res_1 = this.checkCallingOrSelfPermission(permission_1);

        if (res_1 == PackageManager.PERMISSION_GRANTED)
            locationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 200, 1, locationListener);

    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mContext = this;
        locationManager = (LocationManager) mContext.getSystemService(Context.LOCATION_SERVICE);
        ActivityCompat.requestPermissions(this,new String[]{Manifest.permission.ACCESS_FINE_LOCATION}, 1);

        checkLocationPermission();

        isLocationEnabled();

    }

    LocationListener locationListener = new LocationListener() {
        @Override
        public void onLocationChanged(android.location.Location location) {

            if (l_start == null) {
                l_start = location;
                l_end = location;
            } else
                l_end = location;

            double latitude = location.getLatitude();
            double longitude = location.getLongitude();
            double speed = location.getSpeed() * 3.6;
            distance = distance + (l_start.distanceTo(l_end) / 1000.00);

            String distance_msg = "Total distance: " + distance + " km";
            //String time_msg = "Total Time: " + diff + " min";
            String msg = "New Latitude: " + latitude + "New Longitude: " + longitude;
            String speed_msg = "Speed : " + speed + "km/h";

            TextView distance_text = findViewById(R.id.distance);
            distance_text.setText(distance_msg);

            TextView time_text = findViewById(R.id.activity_location);
            time_text.setText(msg);
            TextView speed_text = findViewById(R.id.speed_act);
            speed_text.setText(speed_msg);

            Toast.makeText(mContext, speed_msg, Toast.LENGTH_LONG).show();

            l_start = l_end;
        }

        @Override
        public void onStatusChanged(String provider, int status, Bundle extras) {

        }

        @Override
        public void onProviderEnabled(String provider) {

        }

        @Override
        public void onProviderDisabled(String provider) {

        }
    };


    protected void onResume() {
        super.onResume();
        isLocationEnabled();
    }

    private void isLocationEnabled() {

        if (!locationManager.isProviderEnabled(LocationManager.GPS_PROVIDER)) {
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

