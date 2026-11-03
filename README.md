#Profil – VUT

C++ application for surface profile analysis and characterization

Profil is a Windows desktop application developed in C++ using Microsoft Foundation Classes (MFC).

The application is designed to analyze and characterize surface properties based on measured surface profiles. Its primary purpose is to evaluate surface structures using autocorrelation analysis and other numerical methods.

The analysis helps identify and compare different surface types according to their structural properties and their influence on the reflection of sound, light and other physical phenomena.

Main Features

Surface Profile Measurement Data

Loading measured surface profile data from text files.

Processing measured X/Y coordinates.

Sampling profile data using a configurable step.

Visualization of measured surface profiles.

Autocorrelation Analysis

Autocorrelation is the primary analysis method implemented in the application.

Calculation and visualization of the autocorrelation function.

Analysis of correlations within measured surface profiles.

Extraction of characteristic values from autocorrelation curves.

Calculation and visualization of upper and lower autocorrelation envelopes.

These methods support the characterization and comparison of surface structures.

Numerical Analysis

Power spectrum calculation.

Mean arithmetic profile slope calculation.

Profile value distribution analysis.

Calculation of surface profile parameters, including Ra, Rq, Rz and Rm.

Digital Filtering

Gaussian filtering.

Triangular filtering.

Configurable cutoff lengths.

Separation of roughness and waviness components.

Graphical Visualization

Display of measured and filtered profiles.

Autocorrelation and spectrum graphs.

Graph scaling, axes and numeric labels.

Configurable graph colors.

Interactive analysis of graph values.

User Interface

The application uses the MFC framework and a document/view architecture.

It includes a tabbed configuration dialog for analysis settings, filter selection and graph options, as well as dedicated dialogs for autocorrelation analysis and graph color customization.



Technologies

C++

Microsoft Foundation Classes (MFC)

Windows API

Object-oriented programming

Numerical data processing

Autocorrelation and spectral analysis

GDI-based graphical rendering

Project Structure

Files

Description

Profil.cpp / Profil.h

Main application class and application settings

ProfilView.cpp / ProfilView.h

Profile data processing, numerical calculations and visualization

DataMoznosti0.cpp / .h – DataMoznosti4.cpp / .h

Property pages forming a tabbed dialog for configuring analysis and display options

ODecetKorel.cpp / .h

Extraction of values from autocorrelation graphs

SetColor.cpp / .h

Dialog for customizing graph colors
