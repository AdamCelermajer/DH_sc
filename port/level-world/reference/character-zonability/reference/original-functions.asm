libDungeonHunter2.so SHA-256 36498eb8180ffb74759e6305e9596db999f18583d460f3b8534abcb6022f5e80

_ZNK9Character9IsZonableEv @ 0x3a36e4 (64 bytes, A32)
  3a36e4 push {r4, lr}
  3a36e8 ldr r3, [r0]
  3a36ec mov r4, r0
  3a36f0 mov lr, pc
  3a36f4 ldr pc, [r3, #0x28]      ; virtual Character::IsPlayer
  3a36f8 cmp r0, #0
  3a36fc beq 0x3a3708
  3a3700 mov r0, #0
  3a3704 pop {r4, pc}
  3a3708 mov r0, r4
  3a370c bl 0x3a3094                ; Character::IsFaerie
  3a3710 cmp r0, #0
  3a3714 bne 0x3a3700
  3a3718 mov r0, r4
  3a371c pop {r4, lr}
  3a3720 b 0x38ab60                ; GameObject::MeetCondition

_ZNK9Character8IsFaerieEv @ 0x3a3094 (24 bytes, A32)
  3a3094 push {r4, lr}
  3a3098 bl 0x3a3054                ; Character::GetCharType
  3a309c cmp r0, #3
  3a30a0 movne r0, #0
  3a30a4 moveq r0, #1
  3a30a8 pop {r4, pc}

_ZNK9Character11GetCharTypeEv @ 0x3a3054 (16 bytes, A32)
  3a3054 push {r4, lr}
  3a3058 bl 0x3a3024                ; Character::GetCharAI
  3a305c ldr r0, [r0, #0x38]       ; captured AI row's type word
  3a3060 pop {r4, pc}

_ZNK10GameObject13MeetConditionEv @ 0x38ab60 (8 bytes, A32)
  38ab60 mov r0, #1
  38ab64 bx lr
