import os
import requests

# List of matrices: (Group, Name)
matrices = [
    ("lp_cre_a", "lp_cre_a"),
    ("LPnetlib", "lp_ken_11"),
    ("LPnetlib", "lp_ken_13"),
    ("LPnetlib", "lp_maros_r7"),
    ("LPnetlib", "lp_cre_d"),
    ("LPnetlib", "lp_ken_18"),
    ("Bai", "af23560"),
    ("Shen", "e40r0100"),
    ("vanHeukelum", "cage11"),
    ("vanHeukelum", "cage12"),
    ("LPnetlib", "lp_standata"),
    ("LPnetlib", "lp_scagr25"),
    ("LPnetlib", "lp_scagr7"),
    ("LPnetlib", "lp_stair"),
    ("LPnetlib", "lp_blend"),
    ("LPnetlib", "lp_vtp_base"),
    ("LPnetlib", "lp_agg"),
    ("LPnetlib", "lp_agg2"),
    ("LPnetlib", "lp_agg3"),
    ("LPnetlib", "lp_bore3d"),
    ("LPnetlib", "lp_israel"),
    ("LPnetlib", "lp_tuff"),
    ("LPnetlib", "lp_adlittle"),
    ("HB", "watt_2"),
    ("HB", "can_256"),
    ("HB", "can_268"),
    ("HB", "can_292"),
    ("HB", "can_634"),
    ("HB", "can_715"),
    ("HB", "can_1054"),
    ("HB", "can_1072"),
    ("HB", "impcol_c"),
    ("HB", "impcol_d"),
    ("HB", "impcol_e"),
    ("HB", "west0067"),
    ("HB", "west0381"),
    ("HB", "west0497"),
    ("HB", "gent113"),
    ("HB", "arc130"),
    ("DIMACS10", "598a"),
    ("DIMACS10", "144"),
    ("DIMACS10", "m14b"),
    ("DIMACS10", "auto"),
]

# Create a folder if not exists
os.makedirs("matrices", exist_ok=True)

base_url = "https://sparse.tamu.edu/mat/{}/{}.mat"

for group, name in matrices:
    url = base_url.format(group, name)
    file_path = os.path.join("matrices", f"{name}.mat")

    if os.path.exists(file_path):
        print(f"✅ {name} already downloaded, skipping.")
        continue

    print(f"⬇️ Downloading {name} from {url} ...")
    try:
        response = requests.get(url, stream=True)
        response.raise_for_status()
        with open(file_path, "wb") as f:
            for chunk in response.iter_content(chunk_size=8192):
                f.write(chunk)
        print(f"✔️ Saved {file_path}")
    except requests.exceptions.RequestException as e:
        print(f"❌ Failed to download {name}: {e}")
