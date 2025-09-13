// src/components/ProcessForm.jsx
import { useState } from "react";
import { addProcess } from "../api";

export default function ProcessForm({ onProcessAdded }) {
  const [pid, setPid] = useState("");
  const [name, setName] = useState("");
  const [arrival, setArrival] = useState("");
  const [burst, setBurst] = useState("");
  const [priority, setPriority] = useState("");

  const handleSubmit = async (e) => {
    e.preventDefault();
    try {
      const res = await addProcess(
        Number(pid),
        name,
        Number(arrival),
        Number(burst),
        Number(priority)
      );
      alert("✅ Process added: " + JSON.stringify(res));
      if (onProcessAdded) onProcessAdded();
    } catch (err) {
      console.error("❌ Error adding process:", err);
      alert("Error adding process. Check console.");
    }
  };

  return (
    <form onSubmit={handleSubmit} style={{ marginBottom: "20px" }}>
      <input
        value={pid}
        onChange={(e) => setPid(e.target.value)}
        placeholder="PID"
      />
      <input
        value={name}
        onChange={(e) => setName(e.target.value)}
        placeholder="Name"
      />
      <input
        value={arrival}
        onChange={(e) => setArrival(e.target.value)}
        placeholder="Arrival"
      />
      <input
        value={burst}
        onChange={(e) => setBurst(e.target.value)}
        placeholder="Burst"
      />
      <input
        value={priority}
        onChange={(e) => setPriority(e.target.value)}
        placeholder="Priority"
      />
      <button type="submit">Add Process</button>
    </form>
  );
}
