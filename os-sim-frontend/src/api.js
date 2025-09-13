import axios from "axios";

const BASE_URL = "http://localhost:8080";

// ===== Processes =====
export async function addProcess(proc) {
  const res = await axios.post(`${BASE_URL}/`, proc);
  return res.data;
}

export async function getState() {
  const res = await axios.get(`${BASE_URL}/`);
  return res.data;
}

// ===== Disk =====
export async function addDiskRequest(track) {
  const res = await axios.post(`${BASE_URL}/disk`, { track });
  return res.data;
}

export async function getDiskState() {
  const res = await axios.get(`${BASE_URL}/disk`);
  return res.data;
}

// ===== Paging =====
export async function accessPage(page) {
  const res = await axios.post(`${BASE_URL}/pages`, { page });
  return res.data;
}

export async function getPages() {
  const res = await axios.get(`${BASE_URL}/pages`);
  return res.data;
}
