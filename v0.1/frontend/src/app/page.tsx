"use client";

import { useState, useEffect, useCallback } from "react";

const DEFAULT_HOST = "hackhome.local";

interface DeviceStatus {
  angle: number;
  pressing: boolean;
  rssi: number;
  ip: string;
}

export default function Home() {
  const [host, setHost] = useState(DEFAULT_HOST);
  const [status, setStatus] = useState<DeviceStatus | null>(null);
  const [connected, setConnected] = useState(false);
  const [angle, setAngle] = useState(0);
  const [loading, setLoading] = useState(false);

  const apiUrl = `http://${host}`;

  const fetchStatus = useCallback(async () => {
    try {
      const res = await fetch(`${apiUrl}/api/status`);
      const data: DeviceStatus = await res.json();
      setStatus(data);
      setAngle(data.angle);
      setConnected(true);
    } catch {
      setConnected(false);
      setStatus(null);
    }
  }, [apiUrl]);

  useEffect(() => {
    fetchStatus();
    const interval = setInterval(fetchStatus, 3000);
    return () => clearInterval(interval);
  }, [fetchStatus]);

  const handlePress = async () => {
    setLoading(true);
    try {
      await fetch(`${apiUrl}/api/servo/press`, { method: "POST" });
      setTimeout(fetchStatus, 1000);
    } catch {
      /* ignore */
    }
    setLoading(false);
  };

  const handleAngle = async (newAngle: number) => {
    setAngle(newAngle);
    try {
      await fetch(`${apiUrl}/api/servo/angle`, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
        body: JSON.stringify({ angle: newAngle }),
      });
    } catch {
      /* ignore */
    }
  };

  return (
    <div className="min-h-screen bg-zinc-950 text-zinc-100 p-6 font-[family-name:var(--font-geist-sans)]">
      <div className="max-w-md mx-auto space-y-6">
        {/* Header */}
        <div className="flex items-center justify-between">
          <h1 className="text-2xl font-bold tracking-tight">HackHome</h1>
          <span className="text-xs text-zinc-500 font-mono">v0.1</span>
        </div>

        {/* Connection */}
        <div className="bg-zinc-900 rounded-lg p-4 space-y-3">
          <div className="flex items-center justify-between">
            <span className="text-sm text-zinc-400">Device</span>
            <div className="flex items-center gap-2">
              <div
                className={`w-2 h-2 rounded-full ${
                  connected ? "bg-green-500" : "bg-red-500"
                }`}
              />
              <span className="text-xs text-zinc-500">
                {connected ? "Connected" : "Disconnected"}
              </span>
            </div>
          </div>
          <input
            type="text"
            value={host}
            onChange={(e) => setHost(e.target.value)}
            placeholder="hackhome.local or IP address"
            className="w-full bg-zinc-800 border border-zinc-700 rounded px-3 py-2 text-sm font-mono focus:outline-none focus:border-zinc-500"
          />
          {status && (
            <div className="flex gap-4 text-xs text-zinc-500 font-mono">
              <span>IP: {status.ip}</span>
              <span>RSSI: {status.rssi}dBm</span>
            </div>
          )}
        </div>

        {/* Press Button */}
        <button
          onClick={handlePress}
          disabled={!connected || loading}
          className="w-full bg-white text-zinc-950 font-semibold py-4 rounded-lg text-lg transition-all hover:bg-zinc-200 active:scale-95 disabled:opacity-30 disabled:cursor-not-allowed"
        >
          {loading ? "Pressing..." : "Press"}
        </button>

        {/* Angle Slider */}
        <div className="bg-zinc-900 rounded-lg p-4 space-y-3">
          <div className="flex items-center justify-between">
            <span className="text-sm text-zinc-400">Servo Angle</span>
            <span className="text-lg font-mono font-bold">{angle}°</span>
          </div>
          <input
            type="range"
            min={0}
            max={180}
            value={angle}
            onChange={(e) => handleAngle(Number(e.target.value))}
            disabled={!connected}
            className="w-full accent-white disabled:opacity-30"
          />
          <div className="flex justify-between text-xs text-zinc-600 font-mono">
            <span>0°</span>
            <span>90°</span>
            <span>180°</span>
          </div>
        </div>
      </div>
    </div>
  );
}
