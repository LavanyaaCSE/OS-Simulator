import { useState, useEffect } from "react";
import "./App.css";

export default function App() {
  const [processes, setProcesses] = useState([]);
  const [memory, setMemory] = useState([]);
  const [disk, setDisk] = useState({ head: 0, queue: [] });
  const [pages, setPages] = useState([]);

  // Inputs
  const [pid, setPid] = useState("");
  const [name, setName] = useState("");
  const [arrival, setArrival] = useState("");
  const [burst, setBurst] = useState("");
  const [priority, setPriority] = useState("");
  const [diskRequest, setDiskRequest] = useState("");
  const [pageNumber, setPageNumber] = useState("");

  // Fetch state from backend
  const fetchState = async () => {
    try {
      const res = await fetch("http://localhost:8080/");
      const data = await res.json();
      setProcesses(data.processes || []);
      setMemory(data.memory || []);
    } catch (err) {
      console.error("Error fetching state:", err);
    }

    try {
      const res = await fetch("http://localhost:8080/disk");
      const data = await res.json();
      setDisk({ head: data.head, queue: data.queue });
    } catch (err) {
      console.error("Error fetching disk:", err);
    }

    try {
      const res = await fetch("http://localhost:8080/pages");
      const data = await res.json();
      setPages(data.pages || []);
    } catch (err) {
      console.error("Error fetching pages:", err);
    }
  };

  useEffect(() => {
    fetchState();
    const interval = setInterval(fetchState, 2000);
    return () => clearInterval(interval);
  }, []);

  // Add Process
  const addProcess = async () => {
    if (!pid || !name || !arrival || !burst || !priority) {
      alert("Fill all process fields!");
      return;
    }
    await fetch("http://localhost:8080/", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({
        pid: parseInt(pid),
        name,
        arrival: parseInt(arrival),
        burst: parseInt(burst),
        priority: parseInt(priority),
      }),
    });
    setPid("");
    setName("");
    setArrival("");
    setBurst("");
    setPriority("");
    fetchState();
  };

  // Add Disk Request
  const addDiskRequest = async () => {
    if (!diskRequest) return;
    await fetch("http://localhost:8080/disk", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ track: parseInt(diskRequest) }),
    });
    setDiskRequest("");
    fetchState();
  };

  // Access Page
  const accessPage = async () => {
    if (!pageNumber) return;
    await fetch("http://localhost:8080/pages", {
      method: "POST",
      headers: { "Content-Type": "application/json" },
      body: JSON.stringify({ page: parseInt(pageNumber) }),
    });
    setPageNumber("");
    fetchState();
  };

  return (
    <div className="container">
      <h1>OS Simulator</h1>

      {/* Processes */}
      <section>
        <h2>Processes</h2>
        <div className="form">
          <input
            type="number"
            placeholder="PID"
            value={pid}
            onChange={(e) => setPid(e.target.value)}
          />
          <input
            type="text"
            placeholder="Name"
            value={name}
            onChange={(e) => setName(e.target.value)}
          />
          <input
            type="number"
            placeholder="Arrival"
            value={arrival}
            onChange={(e) => setArrival(e.target.value)}
          />
          <input
            type="number"
            placeholder="Burst"
            value={burst}
            onChange={(e) => setBurst(e.target.value)}
          />
          <input
            type="number"
            placeholder="Priority"
            value={priority}
            onChange={(e) => setPriority(e.target.value)}
          />
          <button onClick={addProcess}>Add Process</button>
        </div>

        <ul>
          {processes.map((p, i) => (
            <li key={i}>
              {p.name} (PID {p.pid}) - Remaining: {p.remaining}, Priority:{" "}
              {p.priority}
            </li>
          ))}
        </ul>
      </section>

      {/* Memory */}
      <section>
        <h2>Memory</h2>
        <ul>
          {memory.map((b, i) => (
            <li key={i}>
              Start: {b.start}, Size: {b.size}, Owner: {b.owner},{" "}
              {b.allocated ? "Allocated" : "Free"}
            </li>
          ))}
        </ul>
      </section>

      {/* Disk */}
      <section>
        <h2>Disk</h2>
        <div className="form">
          <input
            type="number"
            placeholder="Track"
            value={diskRequest}
            onChange={(e) => setDiskRequest(e.target.value)}
          />
          <button onClick={addDiskRequest}>Add Disk Request</button>
        </div>
        <p>Head: {disk.head}</p>
        <p>Queue: {disk.queue && disk.queue.length > 0 ? disk.queue.join(", ") : "Empty"}</p>
      </section>

      {/* Paging */}
      <section>
        <h2>Paging</h2>
        <div className="form">
          <input
            type="number"
            placeholder="Page number"
            value={pageNumber}
            onChange={(e) => setPageNumber(e.target.value)}
          />
          <button onClick={accessPage}>Access Page</button>
        </div>
        <p>Frames: {pages.length > 0 ? pages.join(", ") : "Empty"}</p>
      </section>
    </div>
  );
}
