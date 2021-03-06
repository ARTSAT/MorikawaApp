/*
**      ARTSAT Morikawa Application
**
**      Original Copyright (C) 2013 - 2014 KUBOTA Akihiro.
**                                          http://hemokosa.com
**                                          akihiro.kubota@nifty.com
**      Portions Copyright (C) <year> <author>
**                                          <website>
**                                          <e-mail>
**      All rights reserved.
**
**      Version     Arduino
**      Website     http://artsat.jp/
**      E-mail      info@artsat.jp
**
**      This source code is for Arduino IDE.
**      Arduino 1.0.5
**
**      InvaderMusic.ino
**
**      ------------------------------------------------------------------------
**
**      GNU GENERAL PUBLIC LICENSE (GPLv3)
**
**      This program is free software: you can redistribute it and/or modify
**      it under the terms of the GNU General Public License as published by
**      the Free Software Foundation, either version 3 of the License,
**      or (at your option) any later version.
**      This program is distributed in the hope that it will be useful,
**      but WITHOUT ANY WARRANTY; without even the implied warranty of
**      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
**      See the GNU General Public License for more details.
**      You should have received a copy of the GNU General Public License
**      along with this program. If not, see <http://www.gnu.org/licenses/>.
**
**      このプログラムはフリーソフトウェアです。あなたはこれをフリーソフトウェア財団によって発行された
**      GNU 一般公衆利用許諾書（バージョン 3 か、それ以降のバージョンのうちどれか）が定める条件の下で
**      再頒布または改変することができます。このプログラムは有用であることを願って頒布されますが、
**      *全くの無保証* です。商業可能性の保証や特定目的への適合性は、言外に示されたものも含め全く存在しません。
**      詳しくは GNU 一般公衆利用許諾書をご覧ください。
**      あなたはこのプログラムと共に GNU 一般公衆利用許諾書のコピーを一部受け取っているはずです。
**      もし受け取っていなければ <http://www.gnu.org/licenses/> をご覧ください。
*/

void InvaderMusic_setup(void)
{
    return;
}

void InvaderMusic_loop(void)
{
    unsigned long obcTime;
    unsigned char raw;
    double freq;
    int count, times, duration;
    
    // set tempo
    duration = 1000;
    
    // Theme of INVADER
    static NoteSequence const g_note[] PROGMEM = {
        {NOTE_B4, DURATION_04},
        {NOTE_G4, DURATION_04},
        {NOTE_A4, DURATION_08},
        {NOTE_REST, DURATION_08},
        {NOTE_A4, DURATION_04},
        {NOTE_D5, DURATION_04},
        {NOTE_E5, DURATION_04},
        {NOTE_D5, DURATION_02},
        {NOTE_REST, DURATION_01},
        {NOTE_REST, DURATION_01},
        {NOTE_END}
    };
    
    Morikawa.playNotePGM(g_note);
    
    // intro : OBC time
    Morikawa.getTelemetryTime(TIME_OBCTIME, &obcTime);
    
    raw = lowByte(obcTime >> 24);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/3.0);
    
    raw = lowByte(obcTime >> 16);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/3.0);
    
    raw = lowByte(obcTime >> 8);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/3.0);
    
    raw = lowByte(obcTime);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration);
    
    Morikawa.playFrequency(FREQUENCY_REST, duration);
    
    // group A : Gyro and Magnet
    Morikawa.getTelemetryGyro(GYRO_X, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/2.0);
    
    Morikawa.getTelemetryGyro(GYRO_Y, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/2.0);
    
    Morikawa.getTelemetryGyro(GYRO_Z, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/2.0);
    
    Morikawa.getTelemetryMagnet(MAGNET_X, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/2.0);
    
    Morikawa.getTelemetryMagnet(MAGNET_Y, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/2.0);
    
    Morikawa.getTelemetryMagnet(MAGNET_Z, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/2.0);
    
    Morikawa.playFrequency(FREQUENCY_REST, duration);
    
    
    // group B : Voltage
    Morikawa.getTelemetryCurrent(VOLTAGE_BUS, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/3.0);
    
    Morikawa.getTelemetryCurrent(VOLTAGE_BATTERY, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/3.0);
    
    Morikawa.getTelemetryCurrent(VOLTAGE_SOLAR, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/3.0);
    
    Morikawa.playFrequency(FREQUENCY_REST, duration);
    
    // group C : Current
    Morikawa.getTelemetryCurrent(CURRENT_BUS, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/3.0);
    
    Morikawa.getTelemetryCurrent(CURRENT_BATTERY, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/3.0);
    
    Morikawa.getTelemetryCurrent(CURRENT_SOLAR, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/3.0);
    
    Morikawa.playFrequency(FREQUENCY_REST, duration);
    
    
    Morikawa.getTelemetryCurrent(CURRENT_POWERCPU, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/3.0);
    
    Morikawa.getTelemetryCurrent(CURRENT_MAINCPU, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/3.0);
    
    Morikawa.getTelemetryCurrent(CURRENT_MISSIONCPU, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/3.0);
    
    Morikawa.playFrequency(FREQUENCY_REST, duration);
    
    
    Morikawa.getTelemetryCurrent(CURRENT_BATTERYHEATER, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration);
    
    Morikawa.playFrequency(FREQUENCY_REST, duration);
    
    
    Morikawa.getTelemetryCurrent(CURRENT_TX, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/3.0);
    
    Morikawa.getTelemetryCurrent(CURRENT_CW, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/3.0);
    
    Morikawa.getTelemetryCurrent(CURRENT_RX, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/3.0);
    
    Morikawa.playFrequency(FREQUENCY_REST, duration);
    
    
    // group D : Tempature
    Morikawa.getTelemetryTemperature(TEMPERATURE_BATTERY1, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/3.0);
    
    Morikawa.getTelemetryTemperature(TEMPERATURE_BATTERY2, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/3.0);
    
    Morikawa.getTelemetryTemperature(TEMPERATURE_BATTERY3, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/3.0);
    
    Morikawa.playFrequency(FREQUENCY_REST, duration);
    
    
    Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_PX, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/2.0);
    
    Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_MX, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/2.0);
    
    Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_PY1, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/4.0);
    Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_PY2, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/4.0);
    
    Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_MY1, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/4.0);
    Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_MY2, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/4.0);
    
    Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_PZ1, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/4.0);
    Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_PZ2, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/4.0);
    
    Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_MZ1, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/4.0);
    
    Morikawa.getTelemetryTemperature(TEMPERATURE_SOLAR_MZ2, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration/4.0);
    
    Morikawa.playFrequency(FREQUENCY_REST, duration);
    
    // Coda
    Morikawa.getTelemetryTemperature(TEMPERATURE_POWERCPU, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration);
    
    Morikawa.getTelemetryTemperature(TEMPERATURE_MAINCPU, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration);
    
    Morikawa.getTelemetryTemperature(TEMPERATURE_MISSIONCPU, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration);
    
    Morikawa.getTelemetryTemperature(TEMPERATURE_CWTX, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration);
    
    Morikawa.getTelemetryTemperature(TEMPERATURE_RX, &raw);
    freq = raw2freq(raw);
    Morikawa.playFrequency(freq, duration);
    
    Morikawa.playFrequency(FREQUENCY_REST, duration);
    return;
}

float raw2freq(int raw) {
    
    float freq, base, range;
    
    base = 110.0;
    range = 64.0;
    
    freq = base * pow(2.0, (float)raw/range);
    return freq;
}
