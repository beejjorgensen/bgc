<!-- Beej's guide to C

# vim: ts=4:sw=4:nosi:et:tw=72
-->

# Atomics

## General usage

Just talk about atomic variables and assume seq con

Atomic operators

## Lock free

## Synchronizing

* No reordering
* All writes visible to other reads

## Library Functions that automatically synchronize

* thrd_create()
* etc

## Memory Access Order

* seq cst
* acq rel
* acq consume
  * dependencies, kill_dependency()
* relaxed

## Fences
