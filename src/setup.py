"""The setuptools based setup module."""

import pathlib

from setuptools import find_packages
from setuptools import setup

here = pathlib.Path(__file__).parent.resolve()

# Get the long description from the README file
long_description = (here / "README.md").read_text(encoding="utf-8")

setup(
    name="openGCAS",
    version="0.9.1",
    description="Open Source GCAS Library",
    author="openAeroSystems group",
    author_email="loyd-hook@utulsa.edu",
    classifiers=[
        "Development Status :: 3 - Alpha",
        "Intended Audience :: Developers",
        "License :: N/A",
        "Programming Language :: Python :: 3.10",
        "Topic :: Software Development :: Library",
    ],
    keywords="GCAS, Aviation, Systems",
    long_description=long_description,
    long_description_content_type="text/markdown",
    package_dir={"": "src"},
    packages=find_packages(where="src"),
    project_urls={
        "Bug Tracker": "TODO",
        "Documentation": "TODO",
        "Homepage": "TODO",
        "Release History": "TODO",
        "Release Notes": "TODO",
        "Source": "TODO",
    },
    python_requires=">=3.10",
    url="TODO",
)
