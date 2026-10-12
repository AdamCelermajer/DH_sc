# Canonical retained staging

The registered receiver uses `construct_fields(sameObject, stateServices)` before InitPost. It creates the one controller/FSM/event graph without loading model or Lua resources. `bind_initialization_properties(view,error)` requires the view's base/saved/gear/resolved pointers to be the same registered PropertyState; the caller retains that view for the receiver lifetime.

`init_post_fields` can now borrow this graph before Session allocation. Its `live_delayed3ec` callback reads constructor-owned CharAI delayed0 until Session allocation, then reads the sole Session lifecycle field. The source delayed store and final InitScriptProcess gate reread this callback after intervening helpers.

At the genuine GetCharAI/LoadScriptProcess point, after LoadBase and Recalc, `construct_script` creates the Session from the current same properties and adopts the existing delayed field. `load_script()` still belongs to source LoadScriptProcess. At actual visual resource initialization, `bind_animation(actualResources)` creates the CPU animation owner once. The existing DACT `construct_graph` convenience wrapper composes fields and animation, retaining previous behavior.

This supplies ownership staging, not successful generic InitPost backends. Visual/GameObject InitPost, save loading, FX, sounds, physical/Revive and groups remain mandatory when reached. The host coordinator fixtures explicitly test delayed backing replacement during GetAI and LoadScriptProcess, retaining the source stores and final gate.
