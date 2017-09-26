from PIL import Image

# with open("msf.png") as f:
import pdb;pdb.set_trace()
image = Image.open("../poc.png")
# image.convert("RGB")
image.verify()

