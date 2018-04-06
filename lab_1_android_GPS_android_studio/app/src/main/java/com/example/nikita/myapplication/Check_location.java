package com.example.nikita.myapplication;

import android.app.Service;
import android.content.Intent;
import android.os.Binder;

import android.os.IBinder;

import com.google.android.gms.location.LocationListener;

import android.support.annotation.Nullable;



public class Check_location extends Service {


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

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        return super.onStartCommand(intent, flags, startId);
    }

    @Override
    public boolean onUnbind(Intent intent) {
        //stopLocationUpdates();

        return super.onUnbind(intent);
    }
}
