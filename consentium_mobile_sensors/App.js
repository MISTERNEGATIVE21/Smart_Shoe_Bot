import React, { useState, useEffect, useRef, useCallback } from 'react';
import { StyleSheet, Text, View, TextInput, TouchableOpacity, Platform } from 'react-native';
import * as Location from 'expo-location';
import { Accelerometer, Magnetometer, Pedometer } from 'expo-sensors';

export default function App() {
  const [sendKey, setSendKey] = useState('');
  const [boardKey, setBoardKey] = useState('');
  const [sensorData, setSensorData] = useState({
    gps: { latitude: 0, longitude: 0 },
    accelerometer: { x: 0, y: 0, z: 0 },
    magnetometer: { x: 0, y: 0, z: 0 },
    stepCount: 0,
  });

  const postIntervalRef = useRef(null);

  const fetchData = useCallback(async () => {
    try {
      // Fetch GPS data
      let { status } = await Location.requestForegroundPermissionsAsync();
      if (status !== 'granted') {
        console.error('Location permission not granted');
        return;
      }
      const location = await Location.getCurrentPositionAsync({});
      setSensorData(prevState => ({
        ...prevState,
        gps: {
          latitude: location.coords.latitude,
          longitude: location.coords.longitude,
        },
      }));

      // Fetch accelerometer data
      Accelerometer.addListener(result => {
        setSensorData(prevState => ({ ...prevState, accelerometer: result }));
      });

      // Fetch magnetometer data
      Magnetometer.addListener(result => {
        setSensorData(prevState => ({ ...prevState, magnetometer: result }));
      });

      // Fetch step count
const today = new Date();
today.setHours(0, 0, 0, 0);

const { steps } = await Pedometer.getStepCountAsync(today, new Date());
    } catch (error) {
      console.error('Error fetching data:', error);
    }
  }, []);

  useEffect(() => {
    fetchData(); // Fetch initial sensor data
  }, [fetchData]);

  useEffect(() => {
    postIntervalRef.current = setInterval(() => {
      fetchData(); // Fetch data at regular intervals
    }, 10000);

    return () => {
      clearInterval(postIntervalRef.current);
    };
  }, [fetchData]);

  const sendData = async () => {
    try {
      const url = `https://consentiuminc.online/api/board/updatedata/?key=${sendKey}&boardkey=${boardKey}`;
     const payload = {
        sensors: {
          sensorData: [
            { info: 'Latitude', data: sensorData.gps.latitude.toFixed(6) },
            { info: 'Longitude', data: sensorData.gps.longitude.toFixed(6) },
            { info: 'Accelerometer', data: JSON.stringify(formatSensorData(sensorData.accelerometer)) },
            { info: 'Magnetometer', data: JSON.stringify(formatSensorData(sensorData.magnetometer)) },
            { info: 'Pedometer', data: sensorData.stepCount.toString() },
          ],
        },
        boardInfo: {
          firmwareVersion: '0.0',
          architecture: Platform.OS || 'unknown',
        }
      };
      console.log('Payload:', JSON.stringify(payload));

      const response = await fetch(url, {
        method: 'POST',
        headers: {
          'Content-Type': 'application/json',
        },
        body: JSON.stringify(payload),
      });

      const data = await response.json();
      console.log('Response from server:', data);
    } catch (error) {
      console.error('Error sending data:', error);
    }
  };

  const formatSensorData = data => ({
    x: data.x.toFixed(2),
    y: data.y.toFixed(2),
    z: data.z.toFixed(2),
  });

  return (
    <View style={styles.container}>
      <TextInput
        style={styles.input}
        placeholder="Enter sendKey"
        onChangeText={setSendKey}
        value={sendKey}
      />

      <TextInput
        style={styles.input}
        placeholder="Enter boardKey"
        onChangeText={setBoardKey}
        value={boardKey}
      />

      <TouchableOpacity style={styles.button} onPress={sendData}>
        <Text>Send Data</Text>
      </TouchableOpacity>
    </View>
  );
}

const styles = StyleSheet.create({
  container: {
    flex: 1,
    justifyContent: 'center',
    alignItems: 'center',
    padding: 16,
  },
  input: {
    height: 40,
    borderColor: 'gray',
    borderWidth: 1,
    margin: 10,
    padding: 10,
    borderRadius: 5,
    width: '80%',
  },
  button: {
    backgroundColor: 'lightblue',
    padding: 10,
    margin: 10,
    borderRadius: 5,
  },
});
